#include "Simulator.h"

using namespace std;

int main()
{
	cout << "Hello World!" << endl;

	{		
		SimulationParameters simulationParameters;
		Simulator simulator(make_shared<GNetworkGateway>(), simulationParameters);
		thread t1 = thread(&Simulator::LaunchSimulation, &simulator);
		
		{ // change the simulation parameters on the fly
			SimulationParameters simulationParameters2;
			simulationParameters2.N = 20;
			simulationParameters2.p = 25;
			simulationParameters2.T = 30;	
			simulationParameters2.m = 2; 
			simulationParameters2.provider_selection_function = cheapest_first;
			simulationParameters2.irng_prov = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(500, 2), default_random_engine(rand()))); 
			simulationParameters2.irng_req = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(6000, 7000), default_random_engine(rand())));

			this_thread::sleep_for(chrono::seconds(simulationParameters.T/2));
			thread t2 = thread(&Simulator::ChangeSimulationParameters, &simulator, simulationParameters2);
			t2.join();
		}

		t1.join();
	} 

	cout << "Bye!" << endl;
	cin.get();
	return 0;
}


