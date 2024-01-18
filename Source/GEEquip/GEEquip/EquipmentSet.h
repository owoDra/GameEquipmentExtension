// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "Equipment/ActiveEquipmentHandle.h"

#include "GameplayTagContainer.h"

#include "EquipmentSet.generated.h"

class UItemData;
class UEquipmentManagerComponent;


/**
 * Data asset used to collectively add Equipment to EquipmentComponent
 */
UCLASS(BlueprintType, Const)
class GEEQUIP_API UEquipmentSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UEquipmentSet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Validate Data
public:
#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	// Equipment Items
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipments", meta = (ForceInlineRow, Categories = "Equipment.Slot", AssetBundles = "Item, Equipment, Client, Server"))
	TMap<FGameplayTag, TSoftObjectPtr<const UItemData>> Entries;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipments", meta = (Categories = "Equipment.Slot"))
	FGameplayTag DefaultActiveSlotTag;

public:
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, BlueprintPure = false, Category = "Equipments")
	void AddEquipmentItems(UEquipmentManagerComponent* Manager, TArray<FActiveEquipmentHandle>& OutHandles) const;

};
