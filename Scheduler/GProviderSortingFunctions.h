#pragma once
#include "GProvider.h"

inline bool cheapest_first(const shared_ptr<GProvider>  &struct1, const shared_ptr<GProvider> &struct2)
{
	return (struct1->GetCharacteristics().pricePerTask < struct2->GetCharacteristics().pricePerTask);
}

inline bool best_reputation_first (const shared_ptr<GProvider>  &struct1, const shared_ptr<GProvider> &struct2)
{
	return (struct1->GetCharacteristics().reputation > struct1->GetCharacteristics().reputation);
}

inline bool best_efficiency_first(const shared_ptr<GProvider>  &struct1, const shared_ptr<GProvider> &struct2)
{
	return (struct1->GetCharacteristics().efficiency > struct1->GetCharacteristics().efficiency);
}
