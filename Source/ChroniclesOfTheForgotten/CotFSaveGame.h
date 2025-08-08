// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.h"
#include "CotFSaveGame.generated.h"

/**
 *
 */
UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API UCotFSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UCotFSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FPlayerSaveData PlayerData;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	TArray<FBuildableSaveData> SavedBuildables;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float WorldTimeOfDay;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	int32 UserIndex;
};
