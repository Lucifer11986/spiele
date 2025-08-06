// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingData.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONICLESOFTHEFORGOTTEN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemName, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TMap<FName, int32>& GetInventoryContents() const;

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	bool CanCraftItem(const FCraftingRecipe& Recipe) const;

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void CraftItem(const FCraftingRecipe& Recipe);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FName, int32> Inventory;

};
