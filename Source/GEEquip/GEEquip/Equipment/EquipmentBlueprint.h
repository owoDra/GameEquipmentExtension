// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/Blueprint.h"

#include "EquipmentBlueprint.generated.h"

/**
 * Blueprint class used to set equipment information on the editor
 * 
 * Tips:
 *	The equipment factory should pick this for you automatically
 */
UCLASS(BlueprintType)
class GEEQUIP_API UEquipmentBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UEquipmentBlueprint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////
	// Editor Only
public:
#if WITH_EDITOR
	virtual bool SupportedByDefaultBlueprintFactory() const override { return false; }
#endif

};
