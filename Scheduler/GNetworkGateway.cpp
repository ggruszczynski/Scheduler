#include "GNetworkGateway.h"

GNetworkGateway:: GNetworkGateway(list<shared_ptr<GProvider>> &freeProviders_) :freeProviders(freeProviders_) {};

list<shared_ptr<GProvider>> 
GNetworkGateway::GetFreeProviders(size_t count, bool(*comparisonFcn)(const shared_ptr<GProvider> &, const shared_ptr<GProvider> &), const bool &isTimeToQuit)
{
	unique_lock<mutex> lock(mtx);
	list<shared_ptr<GProvider>> result_list;
	cv.wait(lock, [this, &count, &isTimeToQuit] {return count <= freeProviders.size() || isTimeToQuit; });
	if (!isTimeToQuit)	
	{
		this->freeProviders.sort(comparisonFcn);
		auto it = this->freeProviders.begin();
		auto end = next(freeProviders.begin(), count);
		result_list.splice(result_list.begin(), freeProviders, it, end);
	}
	return result_list;
}

size_t GNetworkGateway::CountFreeProviders()
{
	lock_guard<mutex> lock(mtx);
	const auto number_of_free_providers = freeProviders.size();
	return number_of_free_providers;
}

void GNetworkGateway::AddProvider(shared_ptr<GProvider> provider)
{	
	lock_guard<mutex> lock(mtx);

	const bool isXThere = find(freeProviders.begin(), freeProviders.end(), provider) != freeProviders.end();
	if (!isXThere)
	{
		freeProviders.emplace_back(provider);
	}
		
	cv.notify_all();
}

void GNetworkGateway::WakeUp()
{
	// wake up stuck requestors 
	cv.notify_all();
}