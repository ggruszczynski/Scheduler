#include "SimulationParameters.h"

SimulationParameters::SimulationParameters()
{
	N = 10;
	p = 10;
	T = 10;
	m = 3; 
	provider_selection_function = cheapest_first;
	irng_prov = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(400, 3), default_random_engine(rand())));
	irng_req = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(1000, 2000), default_random_engine(rand())));
}

NetworkSize SimulationParameters:: GetNetworkSize() const
{
	int nReq = (p / 100.) * N;  // number of requestors
	int nProv = N - nReq;		// number of providers

	if (nReq < 1 || nProv < 1)
		throw std::invalid_argument("nReq < 1 || nProv < 1");

	if (nProv < m)
		throw std::invalid_argument("nProv < m");

	NetworkSize networkSize(nReq, nProv);
	return networkSize;
}