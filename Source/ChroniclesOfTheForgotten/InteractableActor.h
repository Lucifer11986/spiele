// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActor();

	// Called when the actor is interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* InteractingActor);
	virtual void OnInteract_Implementation(AActor* InteractingActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FString InteractionPrompt;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
