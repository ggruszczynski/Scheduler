#pragma once

#include <random>
#include "IRNG_wrapper.h"

using namespace std;

template <class distribution_type>
class RNG_wrapper : public IRNG_wrapper
{
private:
	distribution_type distribution;
	default_random_engine gen;

public:
	template <class result_type>
	result_type GetTemplatedRandNumber() { return distribution(gen); }

	virtual double GetRandNumber() override { return fabs(distribution(gen)); }

	RNG_wrapper(normal_distribution<> nd, default_random_engine gen_) : distribution(nd), gen(gen_) {}
	RNG_wrapper(uniform_int_distribution<> uid, default_random_engine gen_) : distribution(uid), gen(gen_) {}
	RNG_wrapper(uniform_real_distribution<> urd, default_random_engine gen_) : distribution(urd), gen(gen_) {}

	virtual ~RNG_wrapper() override	{};
};

