// Copyright Epic Games, Inc. All Rights Reserved.

#include "LootContainerActor.h"
#include "Components/StaticMeshComponent.h"
#include "CotFCharacter.h"
#include "InventoryComponent.h"

ALootContainerActor::ALootContainerActor()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    InteractionPrompt = "Loot";
}

void ALootContainerActor::OnInteract_Implementation(AActor* InteractingActor)
{
    ACotFCharacter* PlayerCharacter = Cast<ACotFCharacter>(InteractingActor);
    if (PlayerCharacter && PlayerCharacter->InventoryComponent)
    {
        for (const FItemQuantity& Item : LootTable)
        {
            PlayerCharacter->InventoryComponent->AddItem(Item.ItemID, Item.Quantity);
        }

        // After looting, the container disappears.
        Destroy();
    }
}
