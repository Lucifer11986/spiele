// Copyright Epic Games, Inc. All Rights Reserved.

#include "WorldStateManager.h"
#include "Engine/DirectionalLight.h"

AWorldStateManager::AWorldStateManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	CurrentTimeOfDay = 0.25f; // Start at sunrise
	DayLengthMinutes = 20.0f;
}

void AWorldStateManager::BeginPlay()
{
	Super::BeginPlay();
}

#include "Kismet/KismetMathLibrary.h"

void AWorldStateManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DayLengthMinutes > 0.f)
	{
		float TimeIncrement = DeltaTime / (DayLengthMinutes * 60.0f);
		CurrentTimeOfDay = FMath::Fmod(CurrentTimeOfDay + TimeIncrement, 1.0f);

		if (GEngine)
		{
			FString TimeString = FString::Printf(TEXT("Time: %.2f"), CurrentTimeOfDay);
			GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::White, TimeString);
		}

		// Update Sun Rotation
		if (SunLight)
		{
			// Map time of day (0-1) to sun rotation (0-360 degrees)
			// We subtract 90 degrees to make "noon" (0.5) be at the top (0 degrees pitch)
			float SunAngle = CurrentTimeOfDay * 360.0f - 90.0f;
			FRotator SunRotation = FRotator(SunAngle, -45.0f, 0); // Yaw is fixed for a simple E-W sun path
			SunLight->SetActorRotation(SunRotation);
		}
	}
}
