#pragma once
#include <string>

using namespace std;

class ITask
{
	int taskId;
	string providerStamp; // to identify the provider which solved the task;

protected:
	bool isSolved;

public:
	ITask();
	ITask(int setId);

	bool IsSolved();
	virtual void SolveTask(string stamp) = 0;
	
	virtual ~ITask() = default;
};

