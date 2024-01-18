// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "GameplayTagContainer.h"

#include "EquipmentFragment_SetTagStats.generated.h"


/**
 * EquipmentFragment class to set the Equipment's StatTag
 */
UCLASS()
class GEEQUIP_API UEquipmentFragment_SetTagStats : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UEquipmentFragment_SetTagStats(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:
	//
	// Initial StatTag for this Equipment
	//
	UPROPERTY(EditDefaultsOnly, Category = "SetTagStats", meta = (GameplayTagFilter = "Stat"))
	TMap<FGameplayTag, int32> InitialEquipmentStats;

public:
	virtual void HandleEquipmentGiven();

};
