#pragma once
#include <memory>
#include <random>

#include "RNG_wrapper.h"
#include "GProviderSortingFunctions.h"

using namespace std;

class GProvider;


struct NetworkSize
{
	size_t nReq;  // liczba providerow
	size_t nProv; // liczba requestorow

	NetworkSize(size_t nReq_, size_t nProv) : nReq(nReq_), nProv(nProv) {}
};

struct SimulationParameters
{
	size_t N; // rozmiar sieci
	size_t p; // procent requestorow
	size_t T; // [s] czas trwania symulacji
	size_t m; // liczba liczacych podzadanie

	shared_ptr<IRNG_wrapper> irng_prov ; //[ms] rozklad czasu liczenia zadania
	shared_ptr<IRNG_wrapper> irng_req;	// [ms] rozklad czasu pomiedzy zlecaniem zadan
	bool(*provider_selection_function)(const shared_ptr<GProvider>&, const shared_ptr<GProvider>&); //funkcja oceny stosowana przez requestorow do wyboru najlepszych providerow


	SimulationParameters();

	NetworkSize GetNetworkSize() const;
};
