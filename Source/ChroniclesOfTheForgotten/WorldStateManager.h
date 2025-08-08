// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldStateManager.generated.h"

class ADirectionalLight;

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API AWorldStateManager : public AActor
{
	GENERATED_BODY()

public:
	AWorldStateManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Time")
	ADirectionalLight* SunLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float CurrentTimeOfDay; // 0.0 = midnight, 0.25 = sunrise, 0.5 = noon, 0.75 = sunset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	float DayLengthMinutes;
};
