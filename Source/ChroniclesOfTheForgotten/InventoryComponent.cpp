// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Inventory Component Initialized"));
	}
}

void UInventoryComponent::AddItem(FName ItemName, int32 Quantity)
{
	if (Quantity <= 0) return;

	if (Inventory.Contains(ItemName))
	{
		Inventory[ItemName] += Quantity;
	}
	else
	{
		Inventory.Add(ItemName, Quantity);
	}

	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Added %d x %s. Total: %d"), Quantity, *ItemName.ToString(), Inventory[ItemName]);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Msg);
	}
}

void UInventoryComponent::RemoveItem(FName ItemName, int32 Quantity)
{
	if (Quantity <= 0 || !Inventory.Contains(ItemName)) return;

	int32& CurrentQuantity = Inventory[ItemName];
	CurrentQuantity -= Quantity;

	if (CurrentQuantity <= 0)
	{
		Inventory.Remove(ItemName);
	}
}

const TMap<FName, int32>& UInventoryComponent::GetInventoryContents() const
{
	return Inventory;
}
