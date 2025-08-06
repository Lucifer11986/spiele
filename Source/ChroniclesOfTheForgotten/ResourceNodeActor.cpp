// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceNodeActor.h"
#include "Components/StaticMeshComponent.h"
#include "CotFCharacter.h"

AResourceNodeActor::AResourceNodeActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	ResourceType = "None";
	Quantity = 1;
	InteractionPrompt = "Gather Resource";
}

#include "InventoryComponent.h"

void AResourceNodeActor::OnInteract_Implementation(AActor* InteractingActor)
{
	Super::OnInteract_Implementation(InteractingActor);

	ACotFCharacter* Character = Cast<ACotFCharacter>(InteractingActor);
	if (Character && Character->InventoryComponent)
	{
		Character->InventoryComponent->AddItem(ResourceType, Quantity);
		Destroy();
	}
}
