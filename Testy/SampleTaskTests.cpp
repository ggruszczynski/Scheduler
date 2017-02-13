#include "stdafx.h"
#include "CppUnitTest.h"

//#include <list>

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
		TEST_METHOD(TaskShouldBeSolvable)
		{
			//arrange
			shared_ptr<ITask> itask = make_shared<SampleTask>();
			string  workStamp = "sampleStamp";
			string answer = "Work has been done by " + workStamp + " by a thread with id: ";

			//act
			itask->SolveTask(workStamp);
			shared_ptr<SampleTask> task = dynamic_pointer_cast<SampleTask>(itask);
			auto result = task->GetResult();
		
			//assert
			Assert::IsTrue(result.find(answer) != string::npos);
			Assert::IsTrue(task->IsSolved());
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