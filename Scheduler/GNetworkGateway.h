#pragma once
#include <list>
#include <iostream>
#include <mutex>

#include "GProvider.h"

#include "GProviderSortingFunctions.h"

using namespace std;

class GNetworkGateway
{
private:
	list<shared_ptr<GProvider>> freeProviders;
	mutex mtx;
	condition_variable cv;
public:
	GNetworkGateway() = default;
	GNetworkGateway(list<shared_ptr<GProvider>> &freeProviders_);

	void AddProvider(shared_ptr<GProvider> provider);
	list<shared_ptr<GProvider>> GetFreeProviders(size_t count, bool(*comparisonFcn)(const shared_ptr<GProvider>&, const shared_ptr<GProvider> &), const bool &isTimeToQuit);

	void WakeUp();
	size_t CountFreeProviders();

	~GNetworkGateway() = default;
};

