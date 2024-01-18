// Copyright (C) 2024 owoDra

#pragma once

#include "Component/GFCActorComponent.h"

#include "Equipment/ActiveEquipment.h"
#include "Type/EquipmentMessageTypes.h"

#include "EquipmentManagerComponent.generated.h"

class UEquipment;


/**
 * Delegate to notify changes in EquipmentSlot
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentSlotEventDelegate, FEquipmentSlotChangedMessage, Param);


/**
 * Components for managing Equipment
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class GEEQUIP_API UEquipmentManagerComponent : public UGFCActorComponent
{
	GENERATED_BODY()
public:
	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////////////////////////////
	// Init State
public:
	//
	// Function name used to add this component
	//
	static const FName NAME_ActorFeatureName;

public:
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }

protected:
	virtual void OnRegister() override;

	virtual void HandleChangeInitStateToDataInitialized(UGameFrameworkComponentManager* Manager) override;


	////////////////////////////////////////////////////////////////////////////////////
	// Replication
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;


	////////////////////////////////////////////////////////////////////////////////////
	// Active Equipment Container
private:
	UPROPERTY(Replicated)
	FActiveEquipmentContainer ActiveEquipments;

public:
	UPROPERTY(BlueprintAssignable)
	FEquipmentSlotEventDelegate OnEquipmentSlotChange;

	UPROPERTY(BlueprintAssignable)
	FEquipmentSlotEventDelegate OnActiveEquipmentSlotChange;

public:
	void RegisterReplicatedSubobject(UEquipment* Instance);
	void UnregisterReplicatedSubobject(UEquipment* Instance);


public:
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments", meta = (GameplayTagFilter = "Equipment.Slot"))
	bool AddEquipmentItem(FGameplayTag InSlotTag, const UItemData* InItemData, FActiveEquipmentHandle& OutHandle, bool bEquipImmediately = false);


	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments", meta = (GameplayTagFilter = "Equipment.Slot"))
	void RemoveEquipmentItemByTag(FGameplayTag SlotTag);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments")
	void RemoveEquipmentItemByHandle(FActiveEquipmentHandle Handle);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments")
	void RemoveMultipleEquipmentItemByHandles(const TSet<FActiveEquipmentHandle>& Handles);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments")
	void RemoveAllEquipmentItem();


	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments", meta = (GameplayTagFilter = "Equipment.Slot"))
	bool EquipEquipmentBySlot(FGameplayTag SlotTag);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments")
	bool EquipEquipmentByHandle(FActiveEquipmentHandle Handle);


	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments", meta = (GameplayTagFilter = "Equipment.Slot"))
	void UnequipEquipmentBySlot(FGameplayTag SlotTag);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Equipments")
	void UnequipEquipmentByHandle(FActiveEquipmentHandle Handle);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Equipment")
	bool GetActiveSlotInfo(FEquipmentSlotChangedMessage& SlotInfo) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Equipment", meta = (GameplayTagFilter = "Equipment.Slot"))
	bool GetSlotInfo(FGameplayTag SlotTag, FEquipmentSlotChangedMessage& SlotInfo) const;


	////////////////////////////////////////////////////////////////////////////////////
	// Utilities
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	static UEquipmentManagerComponent* FindEquipmentManagerComponent(const AActor* Actor);

};
