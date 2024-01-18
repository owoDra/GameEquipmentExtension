// Copyright (C) 2024 owoDra

#include "EquipmentFunctionLibrary.h"

#include "EquipmentManagerComponent.h"
#include "EquipmentManagerComponentInterface.h"

#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentFunctionLibrary)


UEquipmentManagerComponent* UEquipmentFunctionLibrary::GetEquipmentManagerComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	if (!Actor)
	{
		return nullptr;
	}

	const auto* EMCI{ Cast<IEquipmentManagerComponentInterface>(Actor) };
	if (EMCI)
	{
		return EMCI->GetEquipmentManagerComponent();
	}

	if (LookForComponent)
	{
		return UEquipmentManagerComponent::FindEquipmentManagerComponent(Actor);
	}

	return nullptr;
}
