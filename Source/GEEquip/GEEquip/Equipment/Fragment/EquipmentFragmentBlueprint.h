// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/Blueprint.h"

#include "EquipmentFragmentBlueprint.generated.h"

/**
 * Blueprint class used to set equipment extra process on the editor
 * 
 * Tips:
 *	The equipment fragment factory should pick this for you automatically
 */
UCLASS(BlueprintType)
class GEEQUIP_API UEquipmentFragmentBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UEquipmentFragmentBlueprint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////
	// Editor Only
public:
#if WITH_EDITOR
	virtual bool SupportedByDefaultBlueprintFactory() const override { return false; }
#endif

};
