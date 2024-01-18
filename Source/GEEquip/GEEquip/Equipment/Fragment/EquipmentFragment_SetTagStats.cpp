// Copyright (C) 2024 owoDra

#include "EquipmentFragment_SetTagStats.h"

#include "GameplayTag/GameplayTagStackInterface.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentFragment_SetTagStats)


UEquipmentFragment_SetTagStats::UEquipmentFragment_SetTagStats(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::ServerOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UEquipmentFragment_SetTagStats::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	int32 Index{ 0 };
	for (const auto& KVP : InitialEquipmentStats)
	{
		const auto& Tag{ KVP.Key };

		if (!Tag.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Tag defined in InitialEquipmentStats[%d] in %s"), Index, *GetNameSafe(this))));
		}

		Index++;
	}

	return Result;
}
#endif


void UEquipmentFragment_SetTagStats::HandleEquipmentGiven()
{
	Super::HandleEquipmentGiven();

	auto* Equipment{ GetEquipment() };

	if (auto* Interface{ Cast<IGameplayTagStackInterface>(Equipment) })
	{
		for (const auto& KVP : InitialEquipmentStats)
		{
			const auto& Tag{ KVP.Key };
			const auto& Count{ KVP.Value };

			Interface->AddStatTagStack(Tag, Count);
		}
	}
}
