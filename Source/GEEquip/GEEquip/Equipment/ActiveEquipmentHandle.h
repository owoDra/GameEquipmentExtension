// Copyright (C) 2024 owoDra

#pragma once

#include "ActiveEquipmentHandle.generated.h"


/**
 * Handle that points to a specific appling equipment.
 * 
 * Tips:
 *	These are globally unique
 */
USTRUCT(BlueprintType)
struct FActiveEquipmentHandle
{
	GENERATED_BODY()
public:
	FActiveEquipmentHandle() : Handle(INDEX_NONE) {}

	/** 
	 * Sets this to a valid handle 
	 */
	void GenerateNewHandle();

private:
	//
	// Unique handle indexing
	//
	UPROPERTY()
	int32 Handle;

public:
	bool operator==(const FActiveEquipmentHandle& Other) const { return Handle == Other.Handle; }
	bool operator!=(const FActiveEquipmentHandle& Other) const { return Handle != Other.Handle; }

public:
	/** 
	 * True if GenerateNewHandle was called on this handle 
	 */
	bool IsValid() const { return Handle != INDEX_NONE; }

	/**
	 * Returns a hash of this handle
	 */
	friend uint32 GetTypeHash(const FActiveEquipmentHandle& ActiveRecipeHandle) { return ::GetTypeHash(ActiveRecipeHandle.Handle); }

	/**
	 * Return this handle as string.
	 */
	FString ToString() const { return IsValid() ? FString::FromInt(Handle) : TEXT("Invalid"); }

};
