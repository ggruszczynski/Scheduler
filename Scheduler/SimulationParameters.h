#pragma once
#include <memory>
#include <random>

#include "RNG_wrapper.h"
#include "GProviderSortingFunctions.h"

using namespace std;

class GProvider;


struct NetworkSize
{
	size_t nReq;  // number of providers
	size_t nProv; // number of requestors

	NetworkSize(size_t nReq_, size_t nProv) : nReq(nReq_), nProv(nProv) {}
};

struct SimulationParameters
{
	size_t N; // size of the network (number of nodes it the network)
	size_t p; // percent of requestors 
	size_t T; // [s] simulation time
	size_t m; // number of providers required to solve task

	shared_ptr<IRNG_wrapper> irng_prov ; //[ms] rng - time required to solve a task 
	shared_ptr<IRNG_wrapper> irng_req;	// [ms] rng - time between announcing next subtasks
	bool(*provider_selection_function)(const shared_ptr<GProvider>&, const shared_ptr<GProvider>&); // provider selection function (cheapest first, etc)


	SimulationParameters();

	NetworkSize GetNetworkSize() const;
};
