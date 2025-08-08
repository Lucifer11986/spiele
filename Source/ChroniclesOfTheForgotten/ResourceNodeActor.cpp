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
		Character->InventoryComponent->AddItem(ResourceType, Quantity);
		Character->StatsComponent->AddExperience(ExperienceValue);
		Destroy();
	}
}
