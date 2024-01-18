// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "GameplayTagContainer.h"

#include "EquipmentFragment_SetAnimLayers.generated.h"


/**
 * EquipmentFragment class to set AnimLayers for a specific mesh
 */
UCLASS()
class GEEQUIP_API UEquipmentFragment_SetAnimLayers : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UEquipmentFragment_SetAnimLayers(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:
	//
	// AnimLayer class that adapts to the Pawn's Mesh when Equipment is Active.
	//
	UPROPERTY(EditDefaultsOnly, Category = "SetAnimLayers", meta = (ForceInlineRow, Categories = "MeshType"))
	TMap<FGameplayTag, TSubclassOf<UAnimInstance>> AnimLayerToApply;

public:
	virtual void HandleEquiped();
	virtual void HandleUnequiped();

};
