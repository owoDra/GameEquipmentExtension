// Copyright (C) 2024 owoDra

#include "EquipmentFragment.h"

#include "Equipment/Equipment.h"
#include "GEEquipLogs.h"

#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentFragment)


UEquipmentFragment::UEquipmentFragment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UEquipmentFragment::HandleEquipmentGiven()
{
	UE_LOG(LogGameCore_Equipment, Log, TEXT("| Given: %s"), *GetNameSafe(this));

	OnEquipmentGiven();
}

void UEquipmentFragment::HandleEquipmentRemove()
{
	UE_LOG(LogGameCore_Equipment, Log, TEXT("| Remove: %s"), *GetNameSafe(this));

	OnEquipmentRemove();
}

void UEquipmentFragment::HandleEquiped()
{
	UE_LOG(LogGameCore_Equipment, Log, TEXT("| Equiped: %s"), *GetNameSafe(this));

	OnEquiped();
}

void UEquipmentFragment::HandleUnequiped()
{
	UE_LOG(LogGameCore_Equipment, Log, TEXT("| Unequiped: %s"), *GetNameSafe(this));

	OnUnequiped();
}


UEquipment* UEquipmentFragment::GetEquipment() const
{
	return GetTypedOuter<UEquipment>();
}

AActor* UEquipmentFragment::GetEquipmentOwner() const
{
	return GetOuter()->GetTypedOuter<AActor>();
}
