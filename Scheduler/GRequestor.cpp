#include "GRequestor.h"

GRequestor::GRequestor(shared_ptr<IRNG_wrapper> irng_, string name_, int m_) : GNode(irng_), name(name_), m(m_) {}

vector<shared_ptr<ITask>> GRequestor::PrepareTasks(size_t m)
{
	vector<shared_ptr<ITask>> tasks;
	tasks.reserve(m);
	for (size_t i = 0; i < m; ++i)
	{
		shared_ptr<ITask> t = make_shared<SampleTask>(i);
		tasks.emplace_back(t);
		this->announcedTasksStore.push_back(t);
	}

	return tasks;
}

void GRequestor::AnnounceTasks()
{
	lock_guard<mutex> lock(mtx);
	list<shared_ptr<GProvider>> selectedProviders;
	if (auto spt = this->networkGateway.lock()) { // Has to be copied into a shared_ptr before usage
		selectedProviders = spt->GetFreeProviders(m, provider_selection_function, time_to_quit);
	}
	else {
		throw exception("buffer is expired\n");
	}

	if (!this->time_to_quit)
	{
		auto tasks = this->PrepareTasks(this->m);
		auto itTask = tasks.begin();
		auto itProv = selectedProviders.begin();
		for (; itProv != selectedProviders.end(); ++itProv, ++itTask)
		{
			(*itProv)->AddWork(*itTask);
		}

		int workTime = round(this->irng->GetRandNumber());  // time to announce the next task 
		this_thread::sleep_for(chrono::milliseconds(workTime));
	}

}


void GRequestor::SetNumberOfRequiredProvidersPerTask(int m_)
{
	lock_guard<mutex> lock(mtx);
	this->m = m_;
}

void GRequestor::SetBuffer(weak_ptr<GNetworkGateway> buffer_)
{
	lock_guard<mutex> lock(mtx);
	this->networkGateway = buffer_;
}

void GRequestor::SetProviderSelectionFunction(bool(*provider_selection_function_)(const shared_ptr<GProvider>&, const shared_ptr<GProvider>&))
{
	lock_guard<mutex> lock(mtx);
	this->provider_selection_function = provider_selection_function_;
};


void GRequestor::Quit()
{
	this->time_to_quit = true;
	if (auto spt = this->networkGateway.lock()) { // Has to be copied into a shared_ptr before usage
		spt->WakeUp();
	}
}

string GRequestor::GetName() const
{
	return this->name;
}

GRequestor::~GRequestor()
{
	//cout << "~" << this->name << endl;
}

