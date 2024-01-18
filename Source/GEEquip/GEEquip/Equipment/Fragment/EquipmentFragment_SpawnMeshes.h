// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "GameplayTagContainer.h"

#include "EquipmentFragment_SpawnMeshes.generated.h"

class USkeletalMeshComponent;
class USkeletalMesh;
class UAnimInstance;


/**
 * Definition data for the component to which the mesh is added
 */
USTRUCT(BlueprintType)
struct FMeshComponentToAddEquipment
{
	GENERATED_BODY()
public:
	FMeshComponentToAddEquipment() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Categories = "MeshType"))
	FGameplayTag MeshTypeTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAddToOwner{ true };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAddToOther{ true };

public:
	bool IsValid() const;

};


/**
 * Actor spawn settings when equipping
 */
USTRUCT(BlueprintType)
struct FEquipmentMeshToSpawn
{
	GENERATED_BODY()
public:
	FEquipmentMeshToSpawn() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> MeshToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAnimInstance> MeshAnimInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttachSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform AttachTransform;

public:
	bool IsValid() const;

};


/**
 * EquipmentFragment class to spawn a skeletal mesh that attaches to a specific mesh
 */
UCLASS()
class GEEQUIP_API UEquipmentFragment_SpawnMeshes : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UEquipmentFragment_SpawnMeshes(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:
	//
	// Definition of SkeletalMesh, which is the appearance of the equipment
	//
	UPROPERTY(EditDefaultsOnly, Category = "SpawnMeshes")
	TArray<FEquipmentMeshToSpawn> MeshesToSpawn;

	//
	// List of tags for the component to which the mesh is to be added
	//
	UPROPERTY(EditDefaultsOnly, Category = "SpawnMeshes")
	TArray<FMeshComponentToAddEquipment> ComponentToAdd;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<USkeletalMeshComponent>> SpawnedMeshes;

public:
	virtual void HandleEquiped();
	virtual void HandleUnequiped();

};
