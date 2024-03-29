// Copyright (C) 2024 owoDra

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "EquipmentFunctionLibrary.generated.h"

class UEquipmentManagerComponent;
class AActor;


UCLASS(MinimalAPI)
class UEquipmentFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	 * Get EquipmentManagerComponent from actor
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (BlueprintInternalUseOnly = "false"))
	static GEEQUIP_API UEquipmentManagerComponent* GetEquipmentManagerComponentFromActor(const AActor* Actor, bool LookForComponent = true);

};
