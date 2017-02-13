#include "ITask.h"

ITask::ITask(){}

ITask::ITask(int setId) : taskId(setId) {}

bool ITask::IsSolved() { return this->isSolved; }
