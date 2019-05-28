// Copyright 2019 IvayloH. All rights reserved.


#include "QuantityItemBase.h"

float AQuantityItemBase::GetItemAmount() const
{
	return bUsePercentage ? ItemAmountPercent : ItemAmount;
}