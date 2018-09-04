#pragma once

#include <memory>
#include <random>

#include "GProvider.h"
#include "GRequestor.h"
#include "GNetworkGateway.h"
#include "RNG_wrapper.h"
#include "SimulationParameters.h"

using namespace std;

class GBuilder 
{
	int buildedProvidersCounter = 0;
	int buildedRequestorsCounter = 0;

public:
	GBuilder() = default;

	ProviderCharacteristics MakeCharacteristics();
	shared_ptr<GProvider> MakeProvider(shared_ptr<GNetworkGateway> networkGateway, shared_ptr<IRNG_wrapper> irng);
	shared_ptr<GRequestor> MakeRequestor(shared_ptr<GNetworkGateway> networkGateway, SimulationParameters simulationParameters);

	~GBuilder() = default;
};

