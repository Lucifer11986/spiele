// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldPopulationManager.generated.h"

class AResourceNodeActor;
class AWolfAiCharacter;

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API AWorldPopulationManager : public AActor
{
	GENERATED_BODY()

public:
	AWorldPopulationManager();

protected:
	virtual void BeginPlay() override;

	void SpawnResources();
	void SpawnWolves();

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AResourceNodeActor> ResourceNodeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AWolfAiCharacter> WolfClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NumberOfResourcesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NumberOfWolvesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnRadius;
};
