#include "GNetworkGateway.h"

GNetworkGateway:: GNetworkGateway(){};
GNetworkGateway:: GNetworkGateway(list<shared_ptr<GProvider>> &freeProviders_) :freeProviders(freeProviders_) {};
GNetworkGateway::~GNetworkGateway(){}

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
		auto end = std::next(freeProviders.begin(), count);
		result_list.splice(result_list.begin(), freeProviders, it, end);
	}
	return result_list;
}

int GNetworkGateway::CountFreeProviders()
{
	lock_guard<mutex> lock(mtx);
	int ileWolnych = freeProviders.size();
	return ileWolnych;
}

void GNetworkGateway::AddProvider(shared_ptr<GProvider> provider)
{	
	lock_guard<mutex> lock(mtx);

	bool isXThere = find(freeProviders.begin(), freeProviders.end(), provider) != freeProviders.end();
	if (!isXThere)
	{
		freeProviders.emplace_back(provider);
	}
		
	cv.notify_all();
}

void GNetworkGateway::WakeUp()
{
	// wake up stucked requestors 
	cv.notify_all();
}