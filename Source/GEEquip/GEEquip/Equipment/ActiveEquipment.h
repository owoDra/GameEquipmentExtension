// Copyright (C) 2024 owoDra

#pragma once

#include "Net/Serialization/FastArraySerializer.h"

#include "Equipment/ActiveEquipmentHandle.h"
#include "Type/EquipmentMessageTypes.h"

#include "GameplayTagContainer.h"

#include "ActiveEquipment.generated.h"

class UItemData;
class UEquipment;
class UEquipmentManagerComponent;


/**
 * Data on added equipment items
 */
USTRUCT(BlueprintType)
struct FActiveEquipment : public FFastArraySerializerItem
{
	GENERATED_BODY()

	friend struct FActiveEquipmentContainer;
	friend class UEquipmentManagerComponent;

public:
	FActiveEquipment() {}

protected:
	//
	// Unique handle to identify this ActiveEquipment
	//
	UPROPERTY()
	FActiveEquipmentHandle Handle;

	//
	// Slots where this equipment was added
	//
	UPROPERTY()
	FGameplayTag Slot;

	//
	// Item data for this equipment item
	//
	UPROPERTY()
	TObjectPtr<const UItemData> ItemData{ nullptr };

	//
	// Instance of this equipment item
	//
	UPROPERTY()
	TObjectPtr<UEquipment> Instance{ nullptr };

	//
	// Whether this equipment item is equipped
	//
	UPROPERTY()
	uint8 bEquiped : 1 { false };

protected:
	/**
	 * Check if it is possible to equip it, and if so, equip it.
	 * 
	 * Tips:
	 *	Returns false if not equipable
	 */
	bool TryEquip();

	/**
	 * Check if it is possible to unequip it, and if so, unequip it.
	 *
	 * Tips:
	 *	Returns false if not unequipable
	 */
	bool TryUnequip();


public:
	/**
	 * Returns debug string of this
	 */
	FString GetDebugString() const;

	/**
	 *　Return whether this is valid or not.
	 */
	bool IsValid() const;

};


/**
 * List of ActiveEquipment
 */
USTRUCT(BlueprintType)
struct FActiveEquipmentContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	friend class UEquipmentManagerComponent;

public:
	FActiveEquipmentContainer() {}

	void RegisterOwner(AActor* InOwner, UEquipmentManagerComponent* InOwnerComponent);

protected:
	//
	// List of currently applied ActiveEquipments
	//
	UPROPERTY()
	TArray<FActiveEquipment> Entries;

	//
	// The owner of this container
	//
	UPROPERTY(NotReplicated)
	TObjectPtr<AActor> Owner{ nullptr };

	//
	// The owner component of this container
	//
	UPROPERTY(NotReplicated)
	TObjectPtr<UEquipmentManagerComponent> OwnerComponent{ nullptr };

	//
	// Whether the component was initialized or not
	//
	UPROPERTY(NotReplicated)
	bool bInitalized{ false };

	//
	// Handle list of ActiveEquipment pending given event execution due to waiting initialization
	//
	UPROPERTY(NotReplicated)
	TSet<FActiveEquipmentHandle> PendingGivenHandles;

	//
	// Handle list of ActiveEquipment pending equiped event execution due to waiting initialization
	//
	UPROPERTY(NotReplicated)
	TSet<FActiveEquipmentHandle> PendingEquipedHandles;


public:
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FActiveEquipment, FActiveEquipmentContainer>(Entries, DeltaParms, *this);
	}


public:
	void HandleInitialized();

protected:
	void AddPendingGivenEquipment(const FActiveEquipment& ActiveEquipment);
	void RemovePendingGivenEquipment(const FActiveEquipment& ActiveEquipment);
	void AddPendingEquipedEquipment(const FActiveEquipment& ActiveEquipment);
	void RemovePendingEquipedEquipment(const FActiveEquipment& ActiveEquipment);


public:
	bool AddEquipmentItem(const FGameplayTag& InSlotTag, const UItemData* InItemData, FActiveEquipmentHandle& OutHandle, bool bEquipImmediately = false);

	void RemoveEquipmentItem(const FActiveEquipmentHandle& Handle);
	void RemoveEquipmentItem(const FGameplayTag& SlotTag);
	void RemoveMultipleEquipmentItems(const TSet<FActiveEquipmentHandle>& Handles);
	void RemoveAllEquipmentItem();

	bool EquipEquipment(const FActiveEquipmentHandle& Handle);
	bool EquipEquipment(const FGameplayTag& SlotTag);
	bool EquipEquipment(FActiveEquipment& ActiveEquipment);

	void UnequipEquipment(const FActiveEquipmentHandle& Handle);
	void UnequipEquipment(const FGameplayTag& SlotTag);
	void UnequipEquipment(FActiveEquipment& ActiveEquipment);

protected:
	void HandleEquipmentGiven(FActiveEquipment& ActiveEquipment);
	void HandleEquipmentRemove(FActiveEquipment& ActiveEquipment);

	void HandleEquipmentEquiped(FActiveEquipment& ActiveEquipment);
	void HandleEquipmentUnequiped(FActiveEquipment& ActiveEquipment);

public:
	bool GetActiveSlotInfo(FEquipmentSlotChangedMessage& SlotInfo) const;
	bool GetSlotInfo(const FGameplayTag& SlotTag, FEquipmentSlotChangedMessage& SlotInfo) const;

protected:
	void BroadcastSlotChangeMessage(
		FGameplayTag SlotTag = FGameplayTag::EmptyTag
		, const UItemData* ItemData = nullptr
		, UEquipment* Instance = nullptr);

	void BroadcastActiveSlotChangeMessage(
		FGameplayTag SlotTag = FGameplayTag::EmptyTag
		, const UItemData* ItemData = nullptr
		, UEquipment* Instance = nullptr);

};

template<>
struct TStructOpsTypeTraits<FActiveEquipmentContainer> : public TStructOpsTypeTraitsBase2<FActiveEquipmentContainer>
{
	enum { WithNetDeltaSerializer = true };
};
