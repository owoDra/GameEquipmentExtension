// Copyright (C) 2024 owoDra

#include "EquipmentManagerComponent.h"

#include "Equipment/Equipment.h"
#include "GEEquipLogs.h"

#include "ItemData.h"

#include "InitState/InitStateTags.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Engine/ActorChannel.h"
#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentManagerComponent)

//////////////////////////////////////////////////////////////////////////////////////////

const FName UEquipmentManagerComponent::NAME_ActorFeatureName("EquipmentManager");

UEquipmentManagerComponent::UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


void UEquipmentManagerComponent::OnRegister()
{
	ActiveEquipments.RegisterOwner(GetOwner(), this);

	Super::OnRegister();
}

void UEquipmentManagerComponent::HandleChangeInitStateToDataInitialized(UGameFrameworkComponentManager* Manager)
{
	ActiveEquipments.HandleInitialized();
}


void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ActiveEquipments, Params);
}

bool UEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	auto bWroteSomething{ Super::ReplicateSubobjects(Channel, Bunch, RepFlags) };

	for (auto& Entry : ActiveEquipments.Entries)
	{
		if (auto Instance{ Entry.Instance })
		{
			bWroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

void UEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (const auto& Entry : ActiveEquipments.Entries)
		{
			if (auto Instance{ Entry.Instance })
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}


void UEquipmentManagerComponent::RegisterReplicatedSubobject(UEquipment* Instance)
{
	if (IsUsingRegisteredSubObjectList())
	{
		AddReplicatedSubObject(Instance);
	}
}

void UEquipmentManagerComponent::UnregisterReplicatedSubobject(UEquipment* Instance)
{
	if (IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(Instance);
	}
}


bool UEquipmentManagerComponent::AddEquipmentItem(FGameplayTag InSlotTag, const UItemData* InItemData, FActiveEquipmentHandle& OutHandle, bool bEquipImmediately)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return false;
	}

	return ActiveEquipments.AddEquipmentItem(InSlotTag, InItemData, OutHandle, bEquipImmediately);
}


void UEquipmentManagerComponent::RemoveEquipmentItemByTag(FGameplayTag SlotTag)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return;
	}

	ActiveEquipments.RemoveEquipmentItem(SlotTag);
}

void UEquipmentManagerComponent::RemoveEquipmentItemByHandle(FActiveEquipmentHandle Handle)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return;
	}

	ActiveEquipments.RemoveEquipmentItem(Handle);
}

void UEquipmentManagerComponent::RemoveMultipleEquipmentItemByHandles(const TSet<FActiveEquipmentHandle>& Handles)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return;
	}

	ActiveEquipments.RemoveMultipleEquipmentItems(Handles);
}

void UEquipmentManagerComponent::RemoveAllEquipmentItem()
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return;
	}

	ActiveEquipments.RemoveAllEquipmentItem();
}


bool UEquipmentManagerComponent::EquipEquipmentBySlot(FGameplayTag SlotTag)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return false;
	}

	return ActiveEquipments.EquipEquipment(SlotTag);
}

bool UEquipmentManagerComponent::EquipEquipmentByHandle(FActiveEquipmentHandle Handle)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return false;
	}

	return ActiveEquipments.EquipEquipment(Handle);
}


void UEquipmentManagerComponent::UnequipEquipmentBySlot(FGameplayTag SlotTag)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return;
	}

	return ActiveEquipments.UnequipEquipment(SlotTag);
}

void UEquipmentManagerComponent::UnequipEquipmentByHandle(FActiveEquipmentHandle Handle)
{
	// Suspend if has not authority

	if (!HasAuthority())
	{
		return;
	}

	return ActiveEquipments.UnequipEquipment(Handle);
}


bool UEquipmentManagerComponent::GetActiveSlotInfo(FEquipmentSlotChangedMessage& SlotInfo) const
{
	return ActiveEquipments.GetActiveSlotInfo(SlotInfo);
}

bool UEquipmentManagerComponent::GetSlotInfo(FGameplayTag SlotTag, FEquipmentSlotChangedMessage& SlotInfo) const
{
	return ActiveEquipments.GetSlotInfo(SlotTag, SlotInfo);
}


UEquipmentManagerComponent* UEquipmentManagerComponent::FindEquipmentManagerComponent(const AActor* Actor)
{
	return (Actor ? Actor->FindComponentByClass<UEquipmentManagerComponent>() : nullptr);
}

#pragma endregion
