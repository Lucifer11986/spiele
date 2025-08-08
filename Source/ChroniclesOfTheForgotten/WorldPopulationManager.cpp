// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldPopulationManager.h"
#include "ResourceNodeActor.h"
#include "WolfAiCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "CotFSaveGame.h"

AWorldPopulationManager::AWorldPopulationManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Default values
	NumberOfResourcesToSpawn = 50;
	NumberOfWolvesToSpawn = 5;
	SpawnRadius = 2000.0f;
}

void AWorldPopulationManager::BeginPlay()
{
	Super::BeginPlay();

	// Only spawn if it's a new game (no save file exists)
	// Note: A more robust system would be managed by a GameInstance or GameMode state.
	if (!UGameplayStatics::DoesSaveGameExist("DefaultSaveSlot", 0))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("New Game Detected. Populating World..."));
		SpawnResources();
		SpawnWolves();
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("Save Game Detected. Skipping initial population."));
	}
}

void AWorldPopulationManager::SpawnResources()
{
	if (!ResourceNodeClass) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	for (int32 i = 0; i < NumberOfResourcesToSpawn; ++i)
	{
		FNavLocation RandomLocation;
		if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, RandomLocation))
		{
			AResourceNodeActor* NewNode = GetWorld()->SpawnActor<AResourceNodeActor>(ResourceNodeClass, RandomLocation.Location, FRotator::ZeroRotator);
			if (NewNode)
			{
				// Randomly assign resource type
				FName ResourceType = (FMath::RandBool()) ? "Wood" : "Stone";
				int32 Quantity = FMath::RandRange(3, 8);
				NewNode->InitializeNode(ResourceType, Quantity);
			}
		}
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Spawned %d Resource Nodes."), NumberOfResourcesToSpawn));
}

void AWorldPopulationManager::SpawnWolves()
{
	if (!WolfClass) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	for (int32 i = 0; i < NumberOfWolvesToSpawn; ++i)
	{
		FNavLocation RandomLocation;
		if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, RandomLocation))
		{
			GetWorld()->SpawnActor<AWolfAiCharacter>(WolfClass, RandomLocation.Location, FRotator::ZeroRotator);
		}
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Spawned %d Wolves."), NumberOfWolvesToSpawn));
}
