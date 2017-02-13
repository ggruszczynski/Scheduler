#pragma once
#include <iostream>
#include <vector>
#include "SampleTask.h"

#include "GNetworkGateway.h"

#include "GNode.h"

using namespace std;

class GRequestor : public GNode
{
	string name;
	size_t m;	  // number providers needed to solve the next subtask 
	list<shared_ptr<ITask>> announcedTasksStore; // store the tasks, maybe you will need the solution sometime...
	bool(*provider_selection_function)(const shared_ptr<GProvider> &, const shared_ptr<GProvider>&) = nullptr;
public:
	GRequestor(shared_ptr<IRNG_wrapper> irng_, string name_, int m_);

	vector<shared_ptr<ITask>> PrepareTasks(size_t m_);
	void AnnounceTasks();

	void SetBuffer(weak_ptr<GNetworkGateway> buffer_);
	void SetNumberOfRequiredProvidersPerTask(int m_);;
	void SetProviderSelectionFunction(bool(*provider_selection_function_)(const shared_ptr<GProvider> &, const shared_ptr<GProvider>&));

	void Quit() override;

	string GetName() const;
	~GRequestor();
};

