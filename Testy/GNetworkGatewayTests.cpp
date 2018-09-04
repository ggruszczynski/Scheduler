#include "stdafx.h"
#include "CppUnitTest.h"


#include <list>
#include "../Scheduler/GRequestor.h"
#include "../Scheduler/GNetworkGateway.h"
#include "../Scheduler/GBuilder.h"
#include "../Scheduler/GProviderSortingFunctions.h"

#include <windows.h>
#include <future>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(GNetworkGatewayTests)
	{
	public:
		TEST_METHOD(GBufferAddGetProvidersWithoutDeadlock)
		{
			//arrange
			shared_ptr<GNetworkGateway> networkGateway = make_shared<GNetworkGateway>();
			GBuilder gproviderBuilder;
		
			for (int i = 0; i < 5; ++i)
			{
				auto irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(0, 2), default_random_engine(rand())));
				gproviderBuilder.MakeProvider(networkGateway, irng);
			}
				
			//act
			auto someProviders1 = async(&GNetworkGateway::GetFreeProviders, networkGateway, 3, cheapest_first, false);
			auto someProviders2 = async(&GNetworkGateway::GetFreeProviders, networkGateway, 7, cheapest_first, false);

			list<shared_ptr<GProvider>> sp1 = someProviders1.get();

			thread t1([](GBuilder& builder, shared_ptr<GNetworkGateway> gateway, int N)
			{
				OutputDebugString(L" providers will be added... \n");
				this_thread::sleep_for(chrono::seconds(3)); 

				for (int i = 0; i < N; ++i)
				{
					auto irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<normal_distribution<>>(normal_distribution<>(0, 2), default_random_engine(rand())));
					builder.MakeProvider(gateway, irng);
				}
					

				OutputDebugString(L" providers added \n");
			}, gproviderBuilder, networkGateway, 6);

			list<shared_ptr<GProvider>> sp2 = someProviders2.get();
			t1.join();

			//assert
			const size_t amount1 = sp1.size();
			const size_t amount2 = sp2.size();
			Assert::AreEqual(size_t(3), amount1);
			Assert::AreEqual(size_t(7), amount2);
			Assert::AreEqual(size_t(1), networkGateway->CountFreeProviders());
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