// Copyright (C) 2024 owoDra

#pragma once

#include "EquipmentPolicyTypes.generated.h"


/**
 * Whether to execute the event of equipped fragments in the network
 */
UENUM(BlueprintType)
enum class EEquipmentFragmentNetExecutionPolicy : uint8
{
	// This Equipment will only both server and client
	Both,

	// This CharacterRecipe will only run on the server
	ServerOnly,

	// This CharacterRecipe will only run on the client
	ClientOnly,

	// This CharacterRecipe will only run on the local client or server that has local control
	LocalOnly
};


/**
 * Whether the equipment can be equipped
 * 
 * Tips:
 *	For example, taking out a weapon and holding it at the ready.
 */
UENUM(BlueprintType)
enum class EEquipmentEquipPolicy : uint8
{
	// This equipment cannot be equipped.
	// 
	// Tips:
	//	It is used for the type of equipment whose effect is activated by possession.
	//	For example, skills, armor, etc.
	CannotBeEquipped	UMETA(DisplayName = "NotEquipable"),

	// This equipment can be equipped.
	// 
	// Tips:
	//	It is used for hand - held equipment such as weapons.
	//	For example, swords, guns, etc.
	CanBeEquipped		UMETA(DisplayName = "Equipable")
};
