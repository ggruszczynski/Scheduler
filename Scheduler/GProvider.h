#pragma once

#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "ITask.h"

#include "ProviderCharacteristics.h"
#include "GNode.h"

using namespace std;

class GNetworkGateway;

class GProvider: public std::enable_shared_from_this<GProvider> , public GNode
{
	ProviderCharacteristics characteristics;
	
	condition_variable cv;
	bool isTaskAssigned = false;

	shared_ptr<ITask> currentTask = nullptr;	
	void PutYourSelfToWaitngForWorkList();

public:
	GProvider();
	GProvider(ProviderCharacteristics characteristics_, shared_ptr<IRNG_wrapper> irng_);
	void SetGNetworkGateway(weak_ptr<GNetworkGateway> buffer_);

	void SolveTasks();
	void AddWork(shared_ptr<ITask> someTask);
	void Quit() override;

	ProviderCharacteristics GetCharacteristics();
	void UpdateCharacteristics();
	~GProvider();
};

