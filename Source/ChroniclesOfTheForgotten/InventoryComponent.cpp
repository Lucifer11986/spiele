// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"
#include "CotFCharacter.h"
#include "CharacterStatsComponent.h"

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

bool UInventoryComponent::HasItems(FItemQuantity ItemCheck) const
{
	if (Inventory.Contains(ItemCheck.ItemID))
	{
		return Inventory[ItemCheck.ItemID] >= ItemCheck.Quantity;
	}
	return false;
}

bool UInventoryComponent::CanCraftItem(const FCraftingRecipe& Recipe) const
{
	for (const FItemQuantity& RequiredItem : Recipe.RequiredItems)
	{
		if (!Inventory.Contains(RequiredItem.ItemID) || Inventory[RequiredItem.ItemID] < RequiredItem.Quantity)
		{
			return false;
		}
	}
	return true;
}

void UInventoryComponent::CraftItem(const FCraftingRecipe& Recipe)
{
	if (CanCraftItem(Recipe))
	{
		// Remove required items
		for (const FItemQuantity& RequiredItem : Recipe.RequiredItems)
		{
			RemoveItem(RequiredItem.ItemID, RequiredItem.Quantity);
		}

		// Add output item
		AddItem(Recipe.OutputItem.ItemID, Recipe.OutputItem.Quantity);

		// Grant Experience
		ACotFCharacter* PlayerChar = Cast<ACotFCharacter>(GetOwner());
		if (PlayerChar && PlayerChar->StatsComponent && Recipe.ExperienceValue > 0)
		{
			PlayerChar->StatsComponent->AddExperience(Recipe.ExperienceValue);
		}

		if (GEngine)
		{
			FString Msg = FString::Printf(TEXT("Crafted %s!"), *Recipe.OutputItem.ItemID.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not enough resources to craft item."));
		}
	}
}
