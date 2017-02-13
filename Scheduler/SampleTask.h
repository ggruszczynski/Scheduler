#pragma once
#include <string>
#include <thread>
#include <sstream>
#include <iostream>

#include "ITask.h"

using namespace std;

class SampleTask : public ITask
{
	string result;
public:
	
	SampleTask();
	SampleTask(int setId);

	virtual void SolveTask(string sampleArgument) override;
	string GetResult() const; 

	~SampleTask(); 
};

