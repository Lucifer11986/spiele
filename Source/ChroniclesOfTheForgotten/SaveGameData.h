// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BuildingData.h" // For EBuildableType
#include "SaveGameData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TMap<FName, int32> Inventory;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int32 Level;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float ExperiencePoints;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TArray<FName> UnlockedSkillIDs;
};

USTRUCT(BlueprintType)
struct FBuildableSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FTransform Transform;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	EBuildableType BuildableType;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FName MaterialID;
};
