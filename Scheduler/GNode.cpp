#include "GNode.h"

GNode::GNode(shared_ptr<IRNG_wrapper> irng_): irng(irng_){}

void GNode:: SetRNG(shared_ptr<IRNG_wrapper> irng_)
{

		lock_guard<mutex> lock(mtx);
		this->irng = irng_;

}


bool GNode::IsTimeToQuit() const
{
	return this->time_to_quit;
}
