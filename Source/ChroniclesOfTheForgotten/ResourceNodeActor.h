// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "ResourceNodeActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API AResourceNodeActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	AResourceNodeActor();

	virtual void OnInteract_Implementation(AActor* InteractingActor) override;

	void InitializeNode(FName InResourceType, int32 InQuantity);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	FName ResourceType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ExperienceValue;

};
