#pragma once

#include <stdexcept>

using namespace std;

class NotImplementedException : public exception
{
public:
	// Construct with given error message:
	NotImplementedException(const char * error = "Functionality not yet implemented!")
	{
		this->errorMessage = error;
	}

	// Provided for compatibility with std::exception.
	const char * what() const noexcept override
	{
		return this->errorMessage.c_str();
	}

private:
	string errorMessage;
};