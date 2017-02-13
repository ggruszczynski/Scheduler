#include "GBuilder.h"

GBuilder::GBuilder(){}

ProviderCharacteristics GBuilder::MakeCharacteristics()
{
	ProviderCharacteristics characteristics;
	characteristics.name = "ProviderNo" + std::to_string(buildedProvidersCounter);
	characteristics.pricePerTask = RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(0.001, 5), default_random_engine(rand())).GetRandNumber();
	characteristics.efficiency = RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(0.001, 10), default_random_engine(rand())).GetRandNumber();
	characteristics.reputation = RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(0, 1), default_random_engine(rand())).GetRandNumber();
	++buildedProvidersCounter;
	return characteristics;
}

shared_ptr<GProvider> GBuilder::MakeProvider(shared_ptr<GNetworkGateway> networkGateway, shared_ptr<IRNG_wrapper> irng)
{
	ProviderCharacteristics characteristics = this->MakeCharacteristics();
	shared_ptr<GProvider> pro = make_shared<GProvider>(characteristics, irng);
	
	networkGateway->AddProvider(pro);
	weak_ptr<GNetworkGateway> wbuffer = networkGateway;
	pro->SetGNetworkGateway(wbuffer);

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

GBuilder::~GBuilder(){}
