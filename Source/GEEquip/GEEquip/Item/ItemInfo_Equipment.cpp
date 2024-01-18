﻿// Copyright (C) 2024 owoDra

#include "ItemInfo_Equipment.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ItemInfo_Equipment)


UItemInfo_Equipment::UItemInfo_Equipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


#if WITH_EDITOR
EDataValidationResult UItemInfo_Equipment::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	if (EquipmentClass.IsNull())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("EquipmentClass has not set in [%s] in [%s]"), *GetNameSafe(this), *GetNameSafe(GetOuter()))));
	}

	return Result;
}
#endif
