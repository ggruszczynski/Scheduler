#pragma once

#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <map>

#include "GRequestor.h"
#include "GBuilder.h"
#include "SimulationParameters.h"

using namespace std;

class Simulator
{
	SimulationParameters simulationParameters;
	chrono::time_point<chrono::steady_clock> startTime;
	chrono::time_point<chrono::steady_clock> endTime;

	int req_thread_count = 0;
	int prov_thread_count = 0;

	mutex mtx;
	condition_variable cv;

	GBuilder gBuilder;
	shared_ptr<GNetworkGateway> networkGateway;

	list<shared_ptr<GProvider>> escapedProviders; // store providers which were detached during change of simulation parameters
	list<shared_ptr<GRequestor>> escapedRequestors;

	map<shared_ptr<GProvider>, thread> provThreadPool;
	map<shared_ptr<GRequestor>, thread> reqThreadPool;

	void RunRequestor(shared_ptr<GRequestor> &requestor);
	void RunProvider(shared_ptr<GProvider> &provider);
	void QuitReq();
	void QuitProv();

	void AddProvThreads();
	void AddReqThreads();

	void SubtractReqThread();
	void SubtractProvThread();

	void QuitSimulation();

public:
	Simulator(shared_ptr<GNetworkGateway> const &networkGateway_, SimulationParameters const &simulationParameters);

	void LaunchSimulation();
	void ChangeSimulationParameters(SimulationParameters const &simulationParameters_);

	~Simulator() = default;
};