// Copyright (C) 2024 owoDra

#include "ActiveEquipment.h"

#include "EquipmentManagerComponent.h"
#include "Equipment/Equipment.h"
#include "Item/ItemInfo_Equipment.h"
#include "Type/EquipmentMessageTypes.h"
#include "GameplayTag/GEEquipTags_Message.h"
#include "GEEquipLogs.h"

#include "ItemData.h"

#include "InitState/InitStateTags.h"

#include "Message/GameplayMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ActiveEquipment)


//////////////////////////////////////////////////////////////////////
// FActiveEquipment

#pragma region FActiveEquipment

bool FActiveEquipment::TryEquip()
{
	check(Instance);

	// Suspend and return false if EquipPolicy is "CannotBeEquipped"

	if (Instance->GetEquipPolicy() == EEquipmentEquipPolicy::CannotBeEquipped)
	{
		return false;
	}

	// Suspend if already equiped

	if (bEquiped == true)
	{
		return false;
	}

	// Set equiped

	bEquiped = true;

	return true;
}

bool FActiveEquipment::TryUnequip()
{
	// Suspend if already unequiped

	if (bEquiped == false)
	{
		return false;
	}

	// Set unequiped

	bEquiped = false;

	return true;
}


FString FActiveEquipment::GetDebugString() const
{
	return FString::Printf(TEXT("[%s](Slot: %s, Data: %s, Instance: %s) ")
		, *Handle.ToString()
		, *Slot.GetTagName().ToString()
		, *GetNameSafe(ItemData)
		, *GetNameSafe(Instance));
}

bool FActiveEquipment::IsValid() const
{
	return Handle.IsValid() && Slot.IsValid() && ItemData && Instance;
}

#pragma endregion


//////////////////////////////////////////////////////////////////////
// FActiveEquipmentContainer

#pragma region FActiveEquipmentContainer

void FActiveEquipmentContainer::RegisterOwner(AActor* InOwner, UEquipmentManagerComponent* InOwnerComponent)
{
	check(InOwner);
	check(InOwnerComponent);

	Owner = InOwner;
	OwnerComponent = InOwnerComponent;
}


void FActiveEquipmentContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const auto& Index : RemovedIndices)
	{
		auto& Entry{ Entries[Index] };

		if (Entry.TryUnequip())
		{
			HandleEquipmentUnequiped(Entry);
		}

		HandleEquipmentRemove(Entry);
	}
}

void FActiveEquipmentContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const auto& Index : AddedIndices)
	{
		auto& Entry{ Entries[Index] };

		HandleEquipmentGiven(Entry);
		HandleEquipmentEquiped(Entry);
	}
}

void FActiveEquipmentContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const auto& Index : ChangedIndices)
	{
		auto& Entry{ Entries[Index] };

		HandleEquipmentEquiped(Entry);
		HandleEquipmentUnequiped(Entry);
	}
}


bool FActiveEquipmentContainer::AddEquipmentItem(const FGameplayTag& InSlotTag, const UItemData* InItemData, FActiveEquipmentHandle& OutHandle, bool bEquipImmediately)
{
	check(Owner);
	check(OwnerComponent);

	// Suspend if arguments are invalid

	if (!InItemData || !InSlotTag.IsValid())
	{
		return false;
	}

	// Suspend if No Equipment Info in ItemData

	const auto* EquipmentInfo{ InItemData->FindInfo<UItemInfo_Equipment>() };

	if (!EquipmentInfo)
	{
		return false;
	}

	// Suspend if the slot cannot be added

	const auto& AddableSlots{ EquipmentInfo->GetAddableSlots() };

	if (AddableSlots.IsValid() && !AddableSlots.HasTag(InSlotTag))
	{
		return false;
	}

	// Suspend if Equipment class is invalid

	const auto& EquipmentClass{ EquipmentInfo->GetEquipmentClass() };

	if (!EquipmentClass)
	{
		UE_LOG(LogGameCore_Equipment, Error, TEXT("EquipmentClass has not set in [%s]"), *GetNameSafe(InItemData));
		return false;
	}

	// Remove if already in slot

	RemoveEquipmentItem(InSlotTag);

	// Add new entry

	auto& NewEntry{ Entries.AddDefaulted_GetRef() };
	NewEntry.Handle.GenerateNewHandle();
	NewEntry.Slot = InSlotTag;
	NewEntry.ItemData = InItemData;

	OutHandle = NewEntry.Handle;

	// Create Instance

	NewEntry.Instance = NewObject<UEquipment>(Owner, EquipmentClass);

	OwnerComponent->RegisterReplicatedSubobject(NewEntry.Instance);

	HandleEquipmentGiven(NewEntry);

	// Equip it if it is to be equipped immediately

	if (bEquipImmediately)
	{
		if (NewEntry.TryEquip())
		{
			HandleEquipmentEquiped(NewEntry);
		}
	}
	
	MarkItemDirty(NewEntry);

	return true;
}


void FActiveEquipmentContainer::RemoveEquipmentItem(const FActiveEquipmentHandle& InHandle)
{
	// Suspend if Handle is invalid

	if (!InHandle.IsValid())
	{
		return;
	}

	// Remove by handles

	for (auto It{ Entries.CreateIterator() }; It; ++It)
	{
		auto& Entry{ *It };

		if (Entry.Handle == InHandle)
		{
			if (Entry.TryUnequip())
			{
				HandleEquipmentUnequiped(Entry);
			}

			HandleEquipmentRemove(Entry);

			It.RemoveCurrent();
			break;
		}
	}

	MarkArrayDirty();
}

void FActiveEquipmentContainer::RemoveEquipmentItem(const FGameplayTag& InSlotTag)
{
	// Suspend if tag is invalid

	if (!InSlotTag.IsValid())
	{
		return;
	}

	// Remove by tag

	for (auto It{ Entries.CreateIterator() }; It; ++It)
	{
		auto& Entry{ *It };

		if (Entry.Slot == InSlotTag)
		{
			if (Entry.TryUnequip())
			{
				HandleEquipmentUnequiped(Entry);
			}

			HandleEquipmentRemove(Entry);

			It.RemoveCurrent();
			break;
		}
	}

	MarkArrayDirty();
}

void FActiveEquipmentContainer::RemoveMultipleEquipmentItems(const TSet<FActiveEquipmentHandle>& Handles)
{
	// Suspend if Handles is empty

	if (Handles.IsEmpty())
	{
		return;
	}

	// Remove by handles

	for (auto It{ Entries.CreateIterator() }; It; ++It)
	{
		auto& Entry{ *It };

		if (Handles.Contains(Entry.Handle))
		{
			if (Entry.TryUnequip())
			{
				HandleEquipmentUnequiped(Entry);
			}

			HandleEquipmentRemove(Entry);

			It.RemoveCurrent();
		}
	}

	MarkArrayDirty();
}

void FActiveEquipmentContainer::RemoveAllEquipmentItem()
{
	for (auto It{ Entries.CreateIterator() }; It; ++It)
	{
		auto& Entry{ *It };

		if (Entry.TryUnequip())
		{
			HandleEquipmentUnequiped(Entry);
		}

		HandleEquipmentRemove(Entry);

		It.RemoveCurrent();
	}

	MarkArrayDirty();
}


bool FActiveEquipmentContainer::EquipEquipment(const FActiveEquipmentHandle& Handle)
{
	auto bResult{ false };

	// Suspend if Handle is invalid

	if (!Handle.IsValid())
	{
		return bResult;
	}

	// Find equipment

	int32 OldEquipedIndex{ INDEX_NONE };
	int32 NewEquipedIndex{ INDEX_NONE };

	for (auto It{ Entries.CreateConstIterator() }; It; ++It)
	{
		if (It->Handle == Handle)
		{
			NewEquipedIndex = It.GetIndex();
		}

		if (It->bEquiped)
		{
			OldEquipedIndex = It.GetIndex();
		}
	}

	// Suspend if old and new index are same

	if (OldEquipedIndex == NewEquipedIndex)
	{
		return false;
	}

	// Unequip old equipment

	if (OldEquipedIndex != INDEX_NONE)
	{
		UnequipEquipment(Entries[OldEquipedIndex]);
	}

	// Equip new equipment

	if (NewEquipedIndex != INDEX_NONE)
	{
		bResult = EquipEquipment(Entries[NewEquipedIndex]);
	}

	return bResult;
}

bool FActiveEquipmentContainer::EquipEquipment(const FGameplayTag& SlotTag)
{
	auto bResult{ false };

	// Suspend if tag is invalid

	if (!SlotTag.IsValid())
	{
		return bResult;
	}

	// Find equipment

	int32 OldEquipedIndex{ INDEX_NONE };
	int32 NewEquipedIndex{ INDEX_NONE };

	for (auto It{ Entries.CreateConstIterator() }; It; ++It)
	{
		if (It->Slot == SlotTag)
		{
			NewEquipedIndex = It.GetIndex();
		}

		if (It->bEquiped)
		{
			OldEquipedIndex = It.GetIndex();
		}
	}

	// Suspend if old and new index are same

	if (OldEquipedIndex == NewEquipedIndex)
	{
		return false;
	}

	// Unequip old equipment

	if (OldEquipedIndex != INDEX_NONE)
	{
		UnequipEquipment(Entries[OldEquipedIndex]);
	}

	// Equip new equipment

	if (NewEquipedIndex != INDEX_NONE)
	{
		bResult = EquipEquipment(Entries[NewEquipedIndex]);
	}

	return bResult;
}

bool FActiveEquipmentContainer::EquipEquipment(FActiveEquipment& ActiveEquipment)
{
	if (ActiveEquipment.TryEquip())
	{
		HandleEquipmentEquiped(ActiveEquipment);

		MarkItemDirty(ActiveEquipment);

		return true;
	}

	return false;
}


void FActiveEquipmentContainer::UnequipEquipment(const FActiveEquipmentHandle& Handle)
{
	// Suspend if Handle is invalid

	if (!Handle.IsValid())
	{
		return;
	}

	// Find equipment

	for (auto& Entry : Entries)
	{
		if (Entry.Handle == Handle)
		{
			UnequipEquipment(Entry);
			return;
		}
	}
}

void FActiveEquipmentContainer::UnequipEquipment(const FGameplayTag& SlotTag)
{
	// Suspend if tag is invalid

	if (!SlotTag.IsValid())
	{
		return;
	}

	// Find equipment

	for (auto& Entry : Entries)
	{
		if (Entry.Slot == SlotTag)
		{
			UnequipEquipment(Entry);
			return;
		}
	}
}

void FActiveEquipmentContainer::UnequipEquipment(FActiveEquipment& ActiveEquipment)
{
	if (ActiveEquipment.TryUnequip())
	{
		HandleEquipmentUnequiped(ActiveEquipment);

		MarkItemDirty(ActiveEquipment);
	}
}


void FActiveEquipmentContainer::HandleInitialized()
{
	if (!bInitalized)
	{
		bInitalized = true;

		for (auto& Entry : Entries)
		{
			if (PendingGivenHandles.Contains(Entry.Handle))
			{
				HandleEquipmentGiven(Entry);
			}

			if (PendingEquipedHandles.Contains(Entry.Handle))
			{
				HandleEquipmentEquiped(Entry);
			}
		}

		PendingGivenHandles.Empty();
		PendingEquipedHandles.Empty();
	}
}


void FActiveEquipmentContainer::AddPendingGivenEquipment(const FActiveEquipment& ActiveEquipment)
{
	PendingGivenHandles.Add(ActiveEquipment.Handle);
}

void FActiveEquipmentContainer::RemovePendingGivenEquipment(const FActiveEquipment& ActiveEquipment)
{
	PendingGivenHandles.Remove(ActiveEquipment.Handle);
}

void FActiveEquipmentContainer::AddPendingEquipedEquipment(const FActiveEquipment& ActiveEquipment)
{
	PendingEquipedHandles.Add(ActiveEquipment.Handle);
}

void FActiveEquipmentContainer::RemovePendingEquipedEquipment(const FActiveEquipment& ActiveEquipment)
{
	PendingEquipedHandles.Remove(ActiveEquipment.Handle);
}


void FActiveEquipmentContainer::HandleEquipmentGiven(FActiveEquipment& ActiveEquipment)
{
	if (!bInitalized)
	{
		AddPendingGivenEquipment(ActiveEquipment);
		return;
	}

	auto Instance{ ActiveEquipment.Instance };
	check(Instance);

	Instance->HandleEquipmentGiven();

	BroadcastSlotChangeMessage(ActiveEquipment.Slot, ActiveEquipment.ItemData, ActiveEquipment.Instance);
}

void FActiveEquipmentContainer::HandleEquipmentRemove(FActiveEquipment& ActiveEquipment)
{
	if (!bInitalized)
	{
		RemovePendingGivenEquipment(ActiveEquipment);
		return;
	}

	if (auto Instance{ ActiveEquipment.Instance })
	{
		Instance->HandleEquipmentRemove();
	}

	BroadcastSlotChangeMessage(ActiveEquipment.Slot, nullptr, nullptr);
}

void FActiveEquipmentContainer::HandleEquipmentEquiped(FActiveEquipment& ActiveEquipment)
{
	if (!bInitalized)
	{
		AddPendingEquipedEquipment(ActiveEquipment);
		return;
	}

	auto Instance{ ActiveEquipment.Instance };
	check(Instance);

	if (ActiveEquipment.bEquiped)
	{
		Instance->HandleEquiped();

		BroadcastSlotChangeMessage(ActiveEquipment.Slot, ActiveEquipment.ItemData, ActiveEquipment.Instance);
	}
}

void FActiveEquipmentContainer::HandleEquipmentUnequiped(FActiveEquipment& ActiveEquipment)
{
	if (!bInitalized)
	{
		RemovePendingEquipedEquipment(ActiveEquipment);
		return;
	}

	auto Instance{ ActiveEquipment.Instance };

	if (Instance && !ActiveEquipment.bEquiped)
	{
		Instance->HandleUnequiped();
	}
}


bool FActiveEquipmentContainer::GetActiveSlotInfo(FEquipmentSlotChangedMessage& SlotInfo) const
{
	for (const auto& Entry : Entries)
	{
		if (Entry.bEquiped)
		{
			SlotInfo.OwnerComponent = OwnerComponent;
			SlotInfo.SlotTag = Entry.Slot;
			SlotInfo.Data = Entry.ItemData;
			SlotInfo.Instance = Entry.Instance;

			return true;
		}
	}

	return false;
}

bool FActiveEquipmentContainer::GetSlotInfo(const FGameplayTag& SlotTag, FEquipmentSlotChangedMessage& SlotInfo) const
{
	for (const auto& Entry : Entries)
	{
		if (Entry.Slot == SlotTag)
		{
			SlotInfo.OwnerComponent = OwnerComponent;
			SlotInfo.SlotTag = Entry.Slot;
			SlotInfo.Data = Entry.ItemData;
			SlotInfo.Instance = Entry.Instance;

			return true;
		}
	}

	return false;
}


void FActiveEquipmentContainer::BroadcastSlotChangeMessage(FGameplayTag SlotTag, const UItemData* ItemData, UEquipment* Instance)
{
	check(Owner);
	check(OwnerComponent);

	if (Owner->HasLocalNetOwner())
	{
		FEquipmentSlotChangedMessage Message;
		Message.OwnerComponent = OwnerComponent;
		Message.SlotTag = SlotTag;
		Message.Data = ItemData;
		Message.Instance = Instance;

		auto& MessageSystem{ UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld()) };
		MessageSystem.BroadcastMessage(TAG_Message_Equipment_SlotChange, Message);

		OwnerComponent->OnEquipmentSlotChange.Broadcast(Message);
	}
}

void FActiveEquipmentContainer::BroadcastActiveSlotChangeMessage(FGameplayTag SlotTag, const UItemData* ItemData, UEquipment* Instance)
{
	check(Owner);
	check(OwnerComponent);

	if (Owner->HasLocalNetOwner())
	{
		FEquipmentSlotChangedMessage Message;
		Message.OwnerComponent = OwnerComponent;
		Message.SlotTag = SlotTag;
		Message.Data = ItemData;
		Message.Instance = Instance;

		auto& MessageSystem{ UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld()) };
		MessageSystem.BroadcastMessage(TAG_Message_Equipment_ActiveSlotChange, Message);

		OwnerComponent->OnActiveEquipmentSlotChange.Broadcast(Message);
	}
}

#pragma endregion
