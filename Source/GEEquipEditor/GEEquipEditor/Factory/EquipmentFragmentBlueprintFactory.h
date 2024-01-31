// Copyright (C) 2024 owoDra

#pragma once

#include "Factories/Factory.h"

#include "EquipmentFragmentBlueprintFactory.generated.h"


UCLASS(HideCategories=Object, MinimalAPI)
class UEquipmentFragmentBlueprintFactory : public UFactory
{
	GENERATED_BODY()
public:
	UEquipmentFragmentBlueprintFactory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//
	// The type of blueprint that will be created
	//
	UPROPERTY(EditAnywhere, Category = "EquipmentFragmentBlueprintFactory")
	TEnumAsByte<enum EBlueprintType> BlueprintType;

	//
	// The parent class of the created blueprint
	//
	UPROPERTY(EditAnywhere, Category = "EquipmentFragmentBlueprintFactory")
	TSubclassOf<class UEquipmentFragment> ParentClass;

public:
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
