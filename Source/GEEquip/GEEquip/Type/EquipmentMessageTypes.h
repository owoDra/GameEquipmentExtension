// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTagContainer.h"

#include "EquipmentMessageTypes.generated.h"

class UEquipmentManagerComponent;
class UEquipment;
class UItemData;


/**
 * Message when Equipment registered in EquipmentManagerComponent is changed.
 */
USTRUCT(BlueprintType)
struct GEEQUIP_API FEquipmentSlotChangedMessage
{
	GENERATED_BODY()
public:
	FEquipmentSlotChangedMessage() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<UEquipmentManagerComponent> OwnerComponent{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag SlotTag{ FGameplayTag::EmptyTag };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<const UItemData> Data{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<UEquipment> Instance{ nullptr };

};
