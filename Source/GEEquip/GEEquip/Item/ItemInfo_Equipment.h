// Copyright (C) 2024 owoDra

#pragma once

#include "Info/ItemInfo.h"

#include "GameplayTagContainer.h"

#include "ItemInfo_Equipment.generated.h"

class UEquipment;


/**
 * Item information for equipment
 */
UCLASS(meta = (DisplayName = "Equipment Info"))
class GEEQUIP_API UItemInfo_Equipment : public UItemInfo
{
	GENERATED_BODY()
public:
	UItemInfo_Equipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/////////////////////////////////////////////////////////////////////////////////////
	// Validate Data
public:
#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

public:
	//
	// List of slots to which this equipment item can be added.
	// 
	// Tips:
	//	Can be added to all slots if not specified
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment", meta = (Categories = "Equipment.Slot"))
	FGameplayTagContainer AddableSlots;

	//
	// Definition class of equipment to be added
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment", meta = (AssetBundles = "Equipment, Client, Server"))
	TSoftClassPtr<UEquipment> EquipmentClass;

};
