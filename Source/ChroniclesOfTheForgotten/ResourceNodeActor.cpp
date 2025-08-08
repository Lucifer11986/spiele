// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceNodeActor.h"
#include "Components/StaticMeshComponent.h"
#include "CotFCharacter.h"
#include "InventoryComponent.h"
#include "CharacterStatsComponent.h"

AResourceNodeActor::AResourceNodeActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	ResourceType = "None";
	Quantity = 1;
	ExperienceValue = 5;
	InteractionPrompt = "Gather Resource";
}

void AResourceNodeActor::OnInteract_Implementation(AActor* InteractingActor)
{
	Super::OnInteract_Implementation(InteractingActor);

	ACotFCharacter* Character = Cast<ACotFCharacter>(InteractingActor);
	if (Character && Character->InventoryComponent && Character->StatsComponent)
	{
		int32 ModifiedYield = Character->StatsComponent->ApplyGatherYieldEffects(Quantity);

		Character->InventoryComponent->AddItem(ResourceType, ModifiedYield);
		Character->StatsComponent->AddExperience(ExperienceValue);
		Destroy();
	}
}

void AResourceNodeActor::InitializeNode(FName InResourceType, int32 InQuantity)
{
	ResourceType = InResourceType;
	Quantity = InQuantity;
	// Here you could also change the mesh based on resource type
}
