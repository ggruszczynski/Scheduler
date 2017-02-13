#pragma once

#include <mutex>
#include "IRNG_wrapper.h"

using namespace std;

class GNetworkGateway;

class GNode
{
protected:
	shared_ptr<IRNG_wrapper> irng;
	weak_ptr<GNetworkGateway> networkGateway;

	bool time_to_quit = false;
	mutex mtx;
public:
	GNode() {};
	GNode(shared_ptr<IRNG_wrapper> irng_);

	void SetRNG(shared_ptr<IRNG_wrapper> irng_);

	virtual void Quit() = 0;
	bool IsTimeToQuit() const;

	virtual ~GNode() {};
};