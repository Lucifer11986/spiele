// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "CraftingData.h" // For FItemQuantity
#include "LootContainerActor.generated.h"

/**
 *
 */
UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API ALootContainerActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	ALootContainerActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<FItemQuantity> LootTable;

	virtual void OnInteract_Implementation(AActor* InteractingActor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

};
