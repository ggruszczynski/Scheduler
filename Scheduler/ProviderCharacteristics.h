#pragma once

#include <string>
#include <iostream>
using namespace std;

struct ProviderCharacteristics
{
	string name = "anonymousProvider";
	double pricePerTask = 0;
	double efficiency = 0;
	double reputation = 0;
	int taskCounter = 0;

	long int totalWorkTime = 0;

	inline void Display()
	{
		cout << name << " (" << pricePerTask << ", " << efficiency << ", " << reputation << ") " << taskCounter << endl;
	}
};

