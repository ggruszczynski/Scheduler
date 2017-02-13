#include "Simulator.h"

Simulator::Simulator(shared_ptr<GNetworkGateway> const &networkGateway_, SimulationParameters const &simulationParameters_) :
	networkGateway(networkGateway_), simulationParameters(simulationParameters_)
{};

void Simulator::RunRequestor(shared_ptr<GRequestor> &requestor)
{
	{
		lock_guard<mutex> lock(mtx);
		++req_thread_count;
	}
	while (!requestor->IsTimeToQuit())
	{
		cout << "r: " << requestor->GetName() << " working\n";
		requestor->AnnounceTasks();
	}
	lock_guard<mutex> lock(mtx);
	cout << "r: " << requestor->GetName() << " ended\n";
	--req_thread_count;
	cv.notify_all();
}

void Simulator::RunProvider(shared_ptr<GProvider> &provider)
{
	{
		lock_guard<mutex> lock(mtx);
		++prov_thread_count;
	}
	while (!provider->IsTimeToQuit())
	{
		cout << "p: " << provider->GetCharacteristics().name << " working\n";		
		provider->SolveTasks();
	}
	lock_guard<mutex> lock(mtx);
	cout << "p: " << provider->GetCharacteristics().name << " ended\n";
	--prov_thread_count;
	cv.notify_all();
}

void Simulator::SubtractProvThread()
{
	// just finish the first one ;p
	auto it = this->provThreadPool.begin();
	it->first->Quit();
	it->second.join();
	this->escapedProviders.emplace_back(it->first);
	lock_guard<mutex> lock(mtx);
	this->provThreadPool.erase(it);
}

void Simulator::SubtractReqThread()
{
	// just finish the first one ;p
	auto it = this->reqThreadPool.begin();
	it->first->Quit();
	it->second.join();
	this->escapedRequestors.emplace_back(it->first);
	lock_guard<mutex> lock(mtx);
	this->reqThreadPool.erase(it);
}


void Simulator::AddProvThreads()
{
	lock_guard<mutex> lock(mtx);
	auto pro = this->gBuilder.MakeProvider(this->networkGateway, this->simulationParameters.irng_prov);
	this->provThreadPool.insert(make_pair(pro, thread(&Simulator::RunProvider, this, pro)));
}


void Simulator::AddReqThreads()
{
	lock_guard<mutex> lock(mtx);
	auto req = this->gBuilder.MakeRequestor(this->networkGateway, this->simulationParameters);
	this->reqThreadPool.insert(make_pair(req, thread(&Simulator::RunRequestor, this, req)));
}


void Simulator::ChangeSimulationParameters(SimulationParameters const &simulationParameters_)
{
	// this function is to show that the simulation parameters can be changed on the fly...
	{
		lock_guard<mutex> lock(mtx);
		cout << "--- changing simulation parameters ---\n";
		this->simulationParameters = simulationParameters_;
	}

	auto networkSize = simulationParameters.GetNetworkSize();

	int d_nReq = networkSize.nReq - reqThreadPool.size();
	if (d_nReq >= 0)
	{
		for (size_t i = 0; i < d_nReq; ++i)
			AddReqThreads();
	}
	else
	{
		for (size_t i = 0; i < abs(d_nReq); ++i)
			SubtractReqThread();
	}


	int d_nProv = networkSize.nProv - provThreadPool.size();
	if (d_nProv >= 0)
	{
		for (size_t i = 0; i < d_nProv; ++i)
			AddProvThreads();
	}
	else
	{
		for (size_t i = 0; i < abs(d_nProv); ++i)
			SubtractProvThread();
	}


	lock_guard<mutex> lock(mtx);
	for (auto it = reqThreadPool.begin(); it != reqThreadPool.end(); ++it)
	{
		if (!(*it).first->IsTimeToQuit()) //it is ok, but dont waste time...
		{
			(*it).first->SetNumberOfRequiredProvidersPerTask(simulationParameters.m);
			(*it).first->SetRNG(simulationParameters.irng_req);
			(*it).first->SetProviderSelectionFunction(simulationParameters.provider_selection_function);
		}
	}

	for (auto it = provThreadPool.begin(); it != provThreadPool.end(); ++it)
	{
		if (!(*it).first->IsTimeToQuit())
		{
			(*it).first->SetRNG(simulationParameters.irng_prov);
		}
	}


	this->endTime = this->startTime + chrono::seconds(this->simulationParameters.T);
	cout << "--- changed simulation parameters ---\n";
}

void Simulator::LaunchSimulation()
{
	auto networkSize = simulationParameters.GetNetworkSize();

	for (size_t i = 0; i < networkSize.nProv; ++i)
		AddProvThreads();	// launch providers

	for (size_t i = 0; i < networkSize.nReq; ++i)
		AddReqThreads();	// launch requestors

	{
		unique_lock<mutex> lk(mtx);
		typedef chrono::steady_clock Clock;
		this->startTime = Clock::now();
		this->endTime = this->startTime + chrono::seconds(this->simulationParameters.T);

		while (Clock::now() < this->endTime) // watch out for spurious wakeups
			cv.wait_until(lk, this->endTime);	
	}

	QuitSimulation();
}

void Simulator::QuitSimulation()
{
	QuitReq();
	QuitProv();

	cout << "-------------------simulation finished-------------------\n";
	cout << "ProviderName (cena, wydajnosc, reputacja) liczba policzonych zadan. " << endl;
	for_each(this->provThreadPool.begin(), provThreadPool.end(), [](pair<const shared_ptr<GProvider>, thread>  &p) {
		p.first->GetCharacteristics().Display();
	});

	for_each(this->escapedProviders.begin(), escapedProviders.end(), [](const shared_ptr<GProvider> &p) {
		p->GetCharacteristics().Display();
	});
}

void Simulator::QuitReq()
{
	cout << "--- requestors quitting ---\n";
	for_each(this->reqThreadPool.begin(), reqThreadPool.end(), [](pair<const shared_ptr<GRequestor>, thread>  &r) {
		r.first->Quit();
	});

	for_each(this->reqThreadPool.begin(), reqThreadPool.end(), [](pair<const shared_ptr<GRequestor>, thread>  &r) {
		r.second.join();
	});

	{
		unique_lock<mutex> lk(mtx);
		while (req_thread_count > 0)
			cv.wait(lk);
		cout << "--- requestors quitted ---\n";
	}
}

void Simulator::QuitProv()
{
	cout << "--- providers quitting ---\n";	
	for_each(this->provThreadPool.begin(), provThreadPool.end(), [](pair<const shared_ptr<GProvider>, thread>  &p) {
		p.first->Quit();
	});

	for_each(this->provThreadPool.begin(), provThreadPool.end(), [](pair<const shared_ptr<GProvider>, thread>  &p) {
		p.second.join();
	});

	{
		unique_lock<mutex> lk(mtx);
		while (prov_thread_count > 0)
			cv.wait(lk);
		cout << "--- providers quitted ---\n";
	}
}


Simulator::~Simulator() {};
