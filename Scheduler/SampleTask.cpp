#include "SampleTask.h"

SampleTask::SampleTask(int setId) : ITask(setId) {}
SampleTask::SampleTask() {}
SampleTask::~SampleTask() {}

void SampleTask::SolveTask(string sampleArgument)
{
	auto threadId = this_thread::get_id();
	stringstream ss;
	ss << threadId;
	string mystring = ss.str();

	this->result = "Work has been done by " + sampleArgument + " by a thread with id: " + ss.str() + " \n";
	this->isSolved = true;
}

string SampleTask::GetResult() const { return this->result; }