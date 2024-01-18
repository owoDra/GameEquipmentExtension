// Copyright (C) 2024 owoDra

#include "EquipmentFragment_SetAnimLayers.h"

#include "Character/CharacterMeshAccessorInterface.h"

#include "Components/SkeletalMeshComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentFragment_SetAnimLayers)

//////////////////////////////////////////////////////////////////////////////////////////

UEquipmentFragment_SetAnimLayers::UEquipmentFragment_SetAnimLayers(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::Both;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UEquipmentFragment_SetAnimLayers::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	int32 Index{ 0 };
	for (const auto& KVP : AnimLayerToApply)
	{
		if (!KVP.Key.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid MeshTag defined in AnimLayerToApply[%d] in %s"), Index, *GetNameSafe(this))));
		}

		if (!KVP.Value)
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid AnimLayerClass defined in AnimLayerToApply[%d] in %s"), Index, *GetNameSafe(this))));
		}

		Index++;
	}

	return Result;
}
#endif


void UEquipmentFragment_SetAnimLayers::HandleEquiped()
{
	Super::HandleEquiped();

	if (auto* Owner{ GetEquipmentOwner() })
	{
		for (const auto& KVP : AnimLayerToApply)
		{
			if (auto* Mesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Owner, KVP.Key) })
			{
				Mesh->LinkAnimClassLayers(KVP.Value);
			}
		}
	}
}

void UEquipmentFragment_SetAnimLayers::HandleUnequiped()
{
	Super::HandleUnequiped();

	if (auto* Owner{ GetEquipmentOwner() })
	{
		for (const auto& KVP : AnimLayerToApply)
		{
			if (auto* Mesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Owner, KVP.Key) })
			{
				Mesh->UnlinkAnimClassLayers(KVP.Value);
			}
		}
	}
}
