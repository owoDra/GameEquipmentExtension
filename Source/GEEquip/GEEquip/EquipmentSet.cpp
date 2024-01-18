// Copyright (C) 2024 owoDra

#include "EquipmentSet.h"

#include "EquipmentManagerComponent.h"
#include "Item/ItemInfo_Equipment.h"

#include "ItemData.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentSet)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UEquipmentSet::UEquipmentSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


#if WITH_EDITOR
EDataValidationResult UEquipmentSet::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	int32 Index{ 0 };
	for (const auto& KVP : Entries)
	{
		const auto& SlotTag{ KVP.Key };
		const auto* ItemData
		{
			KVP.Value.IsNull() ? nullptr :
			KVP.Value.IsValid() ? KVP.Value.Get() : KVP.Value.LoadSynchronous()
		};

		if (!SlotTag.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid SlotTag defined in Entries[%d] in %s"), Index, *GetNameSafe(this))));
		}

		if (!ItemData)
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid ItemData defined in Entries[%d] in %s"), Index, *GetNameSafe(this))));
		}
		else
		{
			const auto* EquipmentInfo{ ItemData->FindInfo<UItemInfo_Equipment>() };

			if (!EquipmentInfo)
			{
				Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

				Context.AddError(FText::FromString(FString::Printf(TEXT("There is no EquipmentInfo in ItemData in Entries[%d] in %s"), Index, *GetNameSafe(this))));
			}
		}

		Index++;
	}

	return Result;
}
#endif


void UEquipmentSet::AddEquipmentItems(UEquipmentManagerComponent* Manager, TArray<FActiveEquipmentHandle>& OutHandles) const
{
	if (Manager)
	{
		for (const auto& KVP : Entries)
		{
			const auto& SlotTag{ KVP.Key };
			const auto* ItemData
			{  
				KVP.Value.IsNull() ? nullptr :
				KVP.Value.IsValid() ? KVP.Value.Get() : KVP.Value.LoadSynchronous()
			};

			FActiveEquipmentHandle OutHandle;

			if (Manager->AddEquipmentItem(SlotTag, ItemData, OutHandle))
			{
				OutHandles.Emplace(OutHandle);
			}
		}

		if (DefaultActiveSlotTag.IsValid())
		{
			Manager->EquipEquipmentBySlot(DefaultActiveSlotTag);
		}
	}
}
