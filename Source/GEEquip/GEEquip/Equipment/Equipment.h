// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTag/GameplayTagStack.h"
#include "GameplayTag/GameplayTagStackInterface.h"

#include "Equipment/EquipmentPolicyTypes.h"

#include "Equipment.generated.h"

class UEquipmentFragment;


/**
 * Base class for the concept of equipment
 * 
 * Tips:
 *	Manage status and data related to equipment
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class GEEQUIP_API UEquipment : public UObject, public IGameplayTagStackInterface
{
	GENERATED_BODY()
public:
	UEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/////////////////////////////////////////////////////////////////////////////////////
	// Validate Data
public:
#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
	

	/////////////////////////////////////////////////////////////////////////////////////
	// Replication
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

protected:
#if UE_WITH_IRIS
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS


	/////////////////////////////////////////////////////////////////////////////////////
	// Tag Stat Stack
protected:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

protected:
	virtual FGameplayTagStackContainer* GetStatTags() override { return &StatTags; }
	virtual const FGameplayTagStackContainer* GetStatTagsConst() const override { return &StatTags; }


	/////////////////////////////////////////////////////////////////////////////////////
	// Policies
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Policies")
	EEquipmentEquipPolicy EquipPolicy{ EEquipmentEquipPolicy::CannotBeEquipped };

public:
	EEquipmentEquipPolicy GetEquipPolicy() const { return EquipPolicy; }


	/////////////////////////////////////////////////////////////////////////////////////
	// Fragments
protected:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UEquipmentFragment>> Fragments;


	/////////////////////////////////////////////////////////////////////////////////////
	// Event
public:
	/**
	 * Executed when equipment is created and registered in the manager
	 */
	virtual void HandleEquipmentGiven();

	/**
	 * Executed when equipment is removed from the manager
	 */
	virtual void HandleEquipmentRemove();

	/**
	 * Executed when equipment is equipped
	 */
	virtual void HandleEquiped();

	/**
	 * Executed when equipment is unequipped.
	 */
	virtual void HandleUnequiped();


	/////////////////////////////////////////////////////////////////////////////////////
	// Utilities
public:
	template<typename T>
	T* GetOwner() const
	{
		return GetTypedOuter<T>();
	}

	template<typename T>
	T* GetOwnerChecked() const
	{
		return CastChecked<T>(GetOuter());
	}

};
