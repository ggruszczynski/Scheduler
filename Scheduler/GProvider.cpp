
#include "GProvider.h"
#include "GNetworkGateway.h"

GProvider::GProvider() {}

GProvider::GProvider(ProviderCharacteristics characteristics_, shared_ptr<IRNG_wrapper> irng_) : characteristics(characteristics_), GNode(irng_) {}

void GProvider::SetGNetworkGateway(weak_ptr<GNetworkGateway> networkGateway_) { this->networkGateway = networkGateway_; }

void GProvider::SolveTasks()
{
	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, [this] {return isTaskAssigned || time_to_quit; });

	if (!time_to_quit)
	{
		int workTime = round(this->irng->GetRandNumber());
		this_thread::sleep_for(chrono::milliseconds(workTime));

		try
		{
			this->currentTask->SolveTask(this->characteristics.name);
		}
		catch (exception e)
		{
			cout << "ouch, corrupted task: " << e.what();
		}

		this->characteristics.totalWorkTime += workTime;
		this->isTaskAssigned = false;

		this->UpdateCharacteristics();
		PutYourSelfToWaitngForWorkList();
	}
}

void GProvider::PutYourSelfToWaitngForWorkList()
{
	if (auto spt = this->networkGateway.lock()) { // Has to be copied into a shared_ptr before usage
		shared_ptr<GProvider> me = shared_from_this();
		spt->AddProvider(me);
	}
	else {
		throw exception("networkGateway is expired\n");
	}
}

void GProvider::AddWork(shared_ptr<ITask> someTask) 
{
	std::unique_lock<std::mutex> lock(mtx);

	while (isTaskAssigned) {
		cv.wait(lock);
	}

	this->currentTask = someTask;
	this->isTaskAssigned = true;
	cv.notify_one(); 
}

void GProvider::Quit()
{
	time_to_quit = true;
	cv.notify_all();
}

ProviderCharacteristics GProvider::GetCharacteristics() { return this->characteristics; }

void GProvider::UpdateCharacteristics()
{
	this->characteristics.taskCounter++;
	this->characteristics.efficiency = (double)this->characteristics.taskCounter/ this->characteristics.totalWorkTime;  

	/* 
	 this->characteristics.reputation = ...

	 I decided to skip the 'reputation' in the demo simulator,
	 because I believe that it would cause the sample project to grow significantly
	 and make the code diffucult to understand (like provider disconnecting or skipping tasks in random manner)
	 Trivial 'facebook-like' solution sounds good, however it would requiere some AI from the providers while a random 'like' doesnt have much sense. 
	*/
} 

GProvider::~GProvider()
{
	//cout << "~" << this->characteristics.name << endl;
}