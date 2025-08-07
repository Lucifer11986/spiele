// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingData.h"
#include "BuildableActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API ABuildableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildableActor();

	void InitializeBuildable(FName InMaterialID, EBuildableType InBuildableType, UDataTable* MaterialDataTable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Building")
	FName MaterialID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Building")
	EBuildableType BuildableType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Building")
	float Health;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
