#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>
#include "../Scheduler/GRequestor.h"
#include "../Scheduler/GProvider.h"
#include "../Scheduler/GNetworkGateway.h"
#include "../Scheduler/GBuilder.h"
#include "../Scheduler/GProviderSortingFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{

	TEST_CLASS(ProviderTests)
	{
	public:
		TEST_METHOD(ProviderShouldChangeTheirWorkSpeed)
		{
			//arrange
			shared_ptr<GNetworkGateway> buffer = make_shared<GNetworkGateway>();
			GBuilder gproviderBuilder;

			auto ptr_irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(0, 2), default_random_engine(rand())));
			auto pro = gproviderBuilder.MakeProvider(buffer, ptr_irng);

			auto ptr_irng2 = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(1, 3), default_random_engine(rand())));
			pro->SetRNG(ptr_irng2);

			Assert::AreEqual(size_t(1), buffer->CountFreeProviders());
			list<shared_ptr<GProvider>> selectedProviders = buffer->GetFreeProviders(1, cheapest_first, false);
			Assert::AreEqual(size_t(0), buffer->CountFreeProviders());
		}


		TEST_METHOD(ProviderShouldGoBackToBufferAfterTaskSolving)
		{
			//arrange
			shared_ptr<GNetworkGateway> buffer = make_shared<GNetworkGateway>();
			GBuilder gproviderBuilder;

			const int n = 5; // amount of providers 	
			thread threads[n];
			for (int i = 0; i <n; ++i) {
				auto irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(0, 2), default_random_engine(rand())));
				auto pro = gproviderBuilder.MakeProvider(buffer, irng);
				threads[i] = thread(&GProvider::SolveTasks, pro);
			}

			auto r_irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(1, 2), default_random_engine(rand())));
			GRequestor req(r_irng, "RequestorName", n);
			auto tasks = req.PrepareTasks(n);

			//act
			list<shared_ptr<GProvider>> selectedProviders = buffer->GetFreeProviders(n, cheapest_first, false);
			Assert::AreEqual(n, (int)selectedProviders.size());
			Assert::AreEqual(size_t(0), buffer->CountFreeProviders());

			auto itTask = tasks.begin();
			auto itProv = selectedProviders.begin();
			for (; itProv != selectedProviders.end(); ++itProv, ++itTask)
			{
				(*itProv)->AddWork(*itTask);
			}

			for (int i = 0; i < n; ++i) {
				threads[i].join();
			}

			//assert
			Assert::AreEqual(size_t(n), buffer->CountFreeProviders());
		}


		TEST_METHOD(ProviderShouldStopWaitingForNewTasks)
		{
			//arrange
			shared_ptr<GNetworkGateway> buffer = make_shared<GNetworkGateway>();
			GBuilder gproviderBuilder;

			const size_t n = 5; // amount of providers 	
			thread threads[n];

			for (int i = 0; i <n; ++i) {
				auto irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(0, 2), default_random_engine(rand())));
				auto pro = gproviderBuilder.MakeProvider(buffer, irng);
				threads[i] = thread(&GProvider::SolveTasks, pro);
			}

			//act
			list<shared_ptr<GProvider>> selectedProviders = buffer->GetFreeProviders(n, cheapest_first, false);
			Assert::AreEqual(n, selectedProviders.size());
			for (auto itProv = selectedProviders.begin(); itProv != selectedProviders.end(); ++itProv)
			{
				(*itProv)->Quit();
			}

			for (int i = 0; i < n; ++i) {
				threads[i].join();
			}

			//assert
			Assert::AreEqual(size_t(0), buffer->CountFreeProviders());
		}


		TEST_METHOD(ProvidersShouldSolveTasks)
		{
			//arrange
			const size_t m = 3; // number of providers needed to solve the subtask 

			shared_ptr<GNetworkGateway> networkGateway = make_shared<GNetworkGateway>();
			GBuilder gproviderBuilder;
			thread threads[m];
			for (size_t i = 0; i < m; ++i)
			{
				auto irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(0, 2), default_random_engine(rand())));
				auto pro = gproviderBuilder.MakeProvider(networkGateway, irng);	
				threads[i] = thread(&GProvider::SolveTasks, pro);
			}

			list<shared_ptr<GProvider>> selectedProviders = networkGateway->GetFreeProviders(m, cheapest_first, false);

			auto r_irng = shared_ptr<IRNG_wrapper>(
				new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(1, 2), default_random_engine(rand()))
				);
			GRequestor req(r_irng, "RequestorName", m);
			auto tasks = req.PrepareTasks(m);

			//act
			auto itTask = tasks.begin();
			auto itProv = selectedProviders.begin();
			for (; itProv != selectedProviders.end(); ++itProv, ++itTask)
			{
				(*itProv)->AddWork(*itTask);
			}

			//assert
			selectedProviders = networkGateway->GetFreeProviders(m, cheapest_first, false);  // providers should go back to networkGateway after job, then we re-take them ;p
			itTask = tasks.begin();
			itProv = selectedProviders.begin();
			for (; itProv != selectedProviders.end(); ++itProv, ++itTask)
			{
		
				shared_ptr<SampleTask> task = dynamic_pointer_cast<SampleTask>(*itTask);
				auto result = task->GetResult();
			
				string stamp = (*itProv)->GetCharacteristics().name;
				string answer = "Work has been done by " + stamp + " by a thread with id: ";

				Assert::IsTrue(result.find(answer) != string::npos);
			}

			// clean up
			for (auto itProv = selectedProviders.begin(); itProv != selectedProviders.end(); ++itProv)
				(*itProv)->Quit();
			
			for (int i = 0; i < m; ++i)
				threads[i].join();
			
		}
	};
}

//you simply need to add the.obj or .lib file(s) of the project under test to the dependencies of the test project.
//
//File->New->Project->Visual C++->Native unit test project(create the test project)
//In the new project open the project Properties
// Properites->Configuration Properties, Linker, Input, Additional Dependencies : Add the names(no full path) of the.obj or .lib files.
// If the function / class under test "lives" in a particular.obj(it will be named after the respective.cpp) file 
// and you don't add that .obj you'll get linker errors(that's your error right there)

//	Properties->Configuration Properties, Linker, General, Additional Library Directories : Add the directory path of the.obj or .lib files.
//	Properties->Configuration Properties, VC++ Directories, Include Directories : Add the header(s) directory(ies) of the project under test.
//	Basically, every case is covered in msdn
//
//https://msdn.microsoft.com/en-us/library/hh419385.aspx#objectRef