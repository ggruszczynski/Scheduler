#include "SimulationParameters.h"

SimulationParameters::SimulationParameters()
{
	N = 10;		// network size
	p = 10;		// percentage of requestors
	T = 10;		// [s] simulation time
	m = 3;		// number of required providers per task
	provider_selection_function = cheapest_first;
	irng_prov = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(400, 3), default_random_engine(rand())));
	irng_req = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(1000, 2000), default_random_engine(rand())));
}

NetworkSize SimulationParameters:: GetNetworkSize() const
{
	const auto nReq = static_cast<size_t>(round(N* p / 100.));  // number of requestors
	const size_t nProv = N - nReq;		// number of providers

	if (nReq < 1 || nProv < 1)
		throw invalid_argument("nReq < 1 || nProv < 1");

	if (nProv < m)
		throw invalid_argument("nProv < m");

	const NetworkSize networkSize(nReq, nProv);
	return networkSize;
}