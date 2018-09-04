#include "GBuilder.h"


ProviderCharacteristics GBuilder::MakeCharacteristics()
{
	ProviderCharacteristics characteristics;
	characteristics.name = "ProviderNo" + to_string(buildedProvidersCounter);
	characteristics.pricePerTask = RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(0.001, 5), default_random_engine(rand())).GetRandNumber();
	characteristics.efficiency = RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(0.001, 10), default_random_engine(rand())).GetRandNumber();
	characteristics.reputation = RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(0, 1), default_random_engine(rand())).GetRandNumber();
	++buildedProvidersCounter;
	return characteristics;
}

shared_ptr<GProvider> GBuilder::MakeProvider(shared_ptr<GNetworkGateway> networkGateway_, shared_ptr<IRNG_wrapper> irng)
{
	ProviderCharacteristics characteristics = this->MakeCharacteristics();
	shared_ptr<GProvider> pro = make_shared<GProvider>(characteristics, irng);
	
	networkGateway_->AddProvider(pro);
	const weak_ptr<GNetworkGateway> w_networkGateway = networkGateway_;
	pro->SetGNetworkGateway(w_networkGateway);
	
	return  pro;
}

shared_ptr<GRequestor> GBuilder::MakeRequestor(shared_ptr<GNetworkGateway> networkGateway, SimulationParameters simulationParameters)
{
	shared_ptr<GRequestor> req = make_shared<GRequestor>(simulationParameters.irng_req, "RequestorNo" + to_string(buildedRequestorsCounter), simulationParameters.m);
	req->SetBuffer(networkGateway);
	req->SetProviderSelectionFunction(simulationParameters.provider_selection_function);

	++buildedRequestorsCounter;
	return req;
}

