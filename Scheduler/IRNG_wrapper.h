#pragma once

class IRNG_wrapper
{
public:
	IRNG_wrapper();

	virtual double GetRandNumber() = 0;
	virtual ~IRNG_wrapper() = default;
};

