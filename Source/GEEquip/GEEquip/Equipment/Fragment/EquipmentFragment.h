// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/EquipmentPolicyTypes.h"

#include "EquipmentFragment.generated.h"

class UEquipment;
class AActor;


/**
 * Base class responsible for processing when equipment is added or done to the equipment
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class GEEQUIP_API UEquipmentFragment : public UObject
{
	GENERATED_BODY()
public:
	UEquipmentFragment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/////////////////////////////////////////////////////////////////////////////////////
	// Policies
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Policies")
	EEquipmentFragmentNetExecutionPolicy NetExecutionPolicy{ EEquipmentFragmentNetExecutionPolicy::Both };

public:
	EEquipmentFragmentNetExecutionPolicy GetNetExecutionPolicy() const { return NetExecutionPolicy; }


	/////////////////////////////////////////////////////////////////////////////////////
	// Event
public:
	/**
	 * Executed when equipment is created and registered in the manager
	 */
	virtual void HandleEquipmentGiven();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnEquipmentGiven();

	/**
	 * Executed when equipment is removed from the manager
	 */
	virtual void HandleEquipmentRemove();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnEquipmentRemove();

	/**
	 * Executed when equipment is equipped
	 */
	virtual void HandleEquiped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnEquiped();

	/**
	 * Executed when equipment is unequipped.
	 */
	virtual void HandleUnequiped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment")
	void OnUnequiped();


	/////////////////////////////////////////////////////////////////////////////////////
	// Utilities
public:
	UEquipment* GetEquipment() const;
	AActor* GetEquipmentOwner() const;

	template<typename T>
	T* GetOwner() const
	{
		return Cast<T>(GetEquipmentOwner());
	}
};
