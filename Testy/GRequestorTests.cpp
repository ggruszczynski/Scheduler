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
	TEST_CLASS(SampleTaskTest)
	{
	public:
		TEST_METHOD(RequestorsShouldStopAnnouncingTasks)
		{
			//arrange
			shared_ptr<GNetworkGateway> buffer = make_shared<GNetworkGateway>();
			GBuilder gproviderBuilder;

			list<shared_ptr<GProvider>> providers;
			const size_t nprov = 5; // number of providers
			for (int i = 0; i < nprov; ++i) {
				auto irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(200, 300), default_random_engine(rand())));
				auto pro = gproviderBuilder.MakeProvider(buffer, irng);
				providers.emplace_back(pro); 
			}

			auto r_irng = shared_ptr<IRNG_wrapper>(new RNG_wrapper<uniform_real_distribution<>>(uniform_real_distribution<>(1, 2), default_random_engine(rand())));
			shared_ptr<GRequestor> req = make_shared<GRequestor>(r_irng, "RequestorName", 10);
			req->SetBuffer(buffer);
			req->SetProviderSelectionFunction(cheapest_first);
	
			thread t = thread(&GRequestor::AnnounceTasks, req);
			this_thread::sleep_for(chrono::seconds(5)); // the requestor will try to grab to many providers...
			req->Quit();
			t.join();
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