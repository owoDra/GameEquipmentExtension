// Copyright (C) 2024 owoDra

#include "Equipment.h"

#include "Fragment/EquipmentFragment.h"
#include "EquipmentFunctionLibrary.h"
#include "EquipmentManagerComponent.h"
#include "GEEquipLogs.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(Equipment)

////////////////////////////////////////////////////////////////////////////////

UEquipment::UEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StatTags(this)
{
}

#if WITH_EDITOR 
EDataValidationResult UEquipment::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	// Validate Fragments

	int32 Index{ 0 };
	for (const auto& Fragment : Fragments)
	{
		if (!Fragment)
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Fragment defined in Fragments[%d] in %s"), Index, *GetNameSafe(this))));
		}
		else
		{
			Result = CombineDataValidationResults(Result, Fragment->IsDataValid(Context));
		}

		Index++;
	}

	return Result;
}
#endif // WITH_EDITOR


void UEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, StatTags, Params);
}

#if UE_WITH_IRIS
void UEquipment::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;

	Super::RegisterReplicationFragments(Context, RegistrationFlags);

	// Build descriptors and allocate PropertyReplicationFragments for this object

	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS


// Event

void UEquipment::HandleEquipmentGiven()
{
	auto* ActorOwner{ GetOwnerChecked<AActor>() };
	const auto bHasAuthority{ ActorOwner->HasAuthority() };
	const auto bIsDedicatedServer{ ActorOwner->IsNetMode(ENetMode::NM_DedicatedServer) };
	const auto bIsLocallyControlled{ ActorOwner->HasLocalNetOwner() };
	
	UE_LOG(LogGameCore_Equipment, Log, TEXT("[%s|%s] OnGiven: %s")
		, bHasAuthority ? TEXT("SERVER") : TEXT("CLIENT")
		, bIsLocallyControlled ? TEXT("Local") : TEXT("NotLocal")
		, *GetNameSafe(this));

	for (const auto& Fragment : Fragments)
	{
		const auto ExecutionPolicy{ Fragment->GetNetExecutionPolicy() };

		if ((ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::Both)
			|| (bHasAuthority && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ServerOnly)
			|| (bIsLocallyControlled && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::LocalOnly)
			|| (!bIsDedicatedServer && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ClientOnly))
		{
			Fragment->HandleEquipmentGiven();
		}
	}
}

void UEquipment::HandleEquipmentRemove()
{
	auto* ActorOwner{ GetOwnerChecked<AActor>() };
	const auto bHasAuthority{ ActorOwner->HasAuthority() };
	const auto bIsDedicatedServer{ ActorOwner->IsNetMode(ENetMode::NM_DedicatedServer) };
	const auto bIsLocallyControlled{ ActorOwner->HasLocalNetOwner() };

	UE_LOG(LogGameCore_Equipment, Log, TEXT("[%s|%s] OnRemove: %s")
		, bHasAuthority ? TEXT("SERVER") : TEXT("CLIENT")
		, bIsLocallyControlled ? TEXT("Local") : TEXT("NotLocal")
		, *GetNameSafe(this));

	for (const auto& Fragment : Fragments)
	{
		const auto ExecutionPolicy{ Fragment->GetNetExecutionPolicy() };

		if ((ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::Both)
			|| (bHasAuthority && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ServerOnly)
			|| (bIsLocallyControlled && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::LocalOnly)
			|| (!bIsDedicatedServer && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ClientOnly))
		{
			Fragment->HandleEquipmentRemove();
		}
	}
}

void UEquipment::HandleEquiped()
{
	auto* ActorOwner{ GetOwnerChecked<AActor>() };
	const auto bHasAuthority{ ActorOwner->HasAuthority() };
	const auto bIsDedicatedServer{ ActorOwner->IsNetMode(ENetMode::NM_DedicatedServer) };
	const auto bIsLocallyControlled{ ActorOwner->HasLocalNetOwner() };

	UE_LOG(LogGameCore_Equipment, Log, TEXT("[%s|%s] OnEquip: %s")
		, bHasAuthority ? TEXT("SERVER") : TEXT("CLIENT")
		, bIsLocallyControlled ? TEXT("Local") : TEXT("NotLocal")
		, *GetNameSafe(this));

	for (const auto& Fragment : Fragments)
	{
		const auto ExecutionPolicy{ Fragment->GetNetExecutionPolicy() };

		if ((ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::Both)
			|| (bHasAuthority && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ServerOnly)
			|| (bIsLocallyControlled && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::LocalOnly)
			|| (!bIsDedicatedServer && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ClientOnly))
		{
			Fragment->HandleEquiped();
		}
	}
}

void UEquipment::HandleUnequiped()
{
	auto* ActorOwner{ GetOwnerChecked<AActor>() };
	const auto bHasAuthority{ ActorOwner->HasAuthority() };
	const auto bIsDedicatedServer{ ActorOwner->IsNetMode(ENetMode::NM_DedicatedServer) };
	const auto bIsLocallyControlled{ ActorOwner->HasLocalNetOwner() };

	UE_LOG(LogGameCore_Equipment, Log, TEXT("[%s|%s] OnUnequip: %s")
		, bHasAuthority ? TEXT("SERVER") : TEXT("CLIENT")
		, bIsLocallyControlled ? TEXT("Local") : TEXT("NotLocal")
		, *GetNameSafe(this));

	for (const auto& Fragment : Fragments)
	{
		const auto ExecutionPolicy{ Fragment->GetNetExecutionPolicy() };

		if ((ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::Both)
			|| (bHasAuthority && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ServerOnly)
			|| (bIsLocallyControlled && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::LocalOnly)
			|| (!bIsDedicatedServer && ExecutionPolicy == EEquipmentFragmentNetExecutionPolicy::ClientOnly))
		{
			Fragment->HandleUnequiped();
		}
	}
}
