// Copyright (C) 2024 owoDra

#pragma once

#include "Info/ItemInfo.h"

#include "GameplayTagContainer.h"

#include "ItemInfo_Equipment.generated.h"

class UEquipment;


/**
 * Item information for equipment
 * 
 * Note:
 *	This ItemInfo is always required for the EquipmentManager to recognize this item as equipment.
 * 
 * Tips:
 *	By inheriting this class and overriding GetEquipmentClass(), 
 *	it can be customized to return any Equipment class from other system values.
 * 
 *	This allows, for example, to have an Equipment class for each skin 
 *	and return the corresponding Equipment class for the skin being applied from another system.
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

protected:
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment", meta = (AssetBundles = "Client, Server"))
	TSoftClassPtr<UEquipment> EquipmentClass;

public:
	virtual const FGameplayTagContainer& GetAddableSlots() const { return AddableSlots; }
	virtual TSubclassOf<UEquipment> GetEquipmentClass() const;

};
