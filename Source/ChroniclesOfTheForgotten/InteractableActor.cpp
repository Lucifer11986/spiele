// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionPrompt = "Interact";
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::OnInteract_Implementation(AActor* InteractingActor)
{
	// Default implementation
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s interacted with %s"), *InteractingActor->GetName(), *GetName()));
	}
}
