// Copyright (C) 2024 owoDra

#include "EquipmentFragment_SpawnMeshes.h"

#include "Character/CharacterMeshAccessorInterface.h"

#include "Components/SkeletalMeshComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentFragment_SpawnMeshes)


///////////////////////////////////////////////////////////////////////////////////
// FMeshComponentToAddEquipment

bool FMeshComponentToAddEquipment::IsValid() const
{
	return MeshTypeTag.IsValid();
}


///////////////////////////////////////////////////////////////////////////////////
// FEquipmentMeshToSpawn

bool FEquipmentMeshToSpawn::IsValid() const
{
	return ::IsValid(MeshToSpawn);
}


///////////////////////////////////////////////////////////////////////////////////
// UEquipmentFragment_SpawnMeshes

UEquipmentFragment_SpawnMeshes::UEquipmentFragment_SpawnMeshes(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::ClientOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UEquipmentFragment_SpawnMeshes::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	int32 Index{ 0 };
	for (const auto& Entry : MeshesToSpawn)
	{
		if (!Entry.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Entry defined in MeshesToSpawn[%d] in %s"), Index, *GetNameSafe(this))));
		}

		Index++;
	}

	Index = 0;
	for (const auto& Entry : ComponentToAdd)
	{
		if (!Entry.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Entry defined in ComponentToAdd[%d] in %s"), Index, *GetNameSafe(this))));
		}

		Index++;
	}

	return Result;
}
#endif


void UEquipmentFragment_SpawnMeshes::HandleEquiped()
{
	Super::HandleEquiped();

	if (auto* Owner{ GetEquipmentOwner() })
	{
		const auto bLocallyControlled{ Owner->HasLocalNetOwner() };

		for (const auto& Entry : ComponentToAdd)
		{
			const auto bCanAdd
			{
				(bLocallyControlled && Entry.bAddToOwner) || (!bLocallyControlled && Entry.bAddToOther)
			};

			if (bCanAdd)
			{
				if (auto* Mesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Owner, Entry.MeshTypeTag) })
				{
					const auto bOwnerNoSee{ static_cast<bool>(Mesh->bOwnerNoSee) };
					const auto bOnlyOwnerSee{ static_cast<bool>(Mesh->bOnlyOwnerSee) };
					const auto bHiddenInGame{ static_cast<bool>(Mesh->bHiddenInGame) };
					const auto bCastShadow{ static_cast<bool>(Mesh->CastShadow) };

					for (const auto& SpawnInfo : MeshesToSpawn)
					{
						if (SpawnInfo.MeshToSpawn)
						{
							USkeletalMeshComponent* NewMesh = NewObject<USkeletalMeshComponent>(Owner);
							NewMesh->SetSkeletalMesh(SpawnInfo.MeshToSpawn);
							NewMesh->SetAnimInstanceClass(SpawnInfo.MeshAnimInstance);
							NewMesh->SetRelativeTransform(SpawnInfo.AttachTransform);
							NewMesh->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);
							NewMesh->SetOwnerNoSee(bOwnerNoSee);
							NewMesh->SetOnlyOwnerSee(bOnlyOwnerSee);
							NewMesh->SetHiddenInGame(bHiddenInGame);
							NewMesh->SetCastShadow(bCastShadow);
							NewMesh->RegisterComponent();

							SpawnedMeshes.Add(NewMesh);
						}
					}
				}
			}
		}
	}
}

void UEquipmentFragment_SpawnMeshes::HandleUnequiped()
{
	Super::HandleUnequiped();

	for (const auto& Mesh : SpawnedMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}

	SpawnedMeshes.Empty();
}
