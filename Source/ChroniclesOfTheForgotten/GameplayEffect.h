// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEffect.generated.h"

/**
 * Base class for all gameplay effects triggered by skills.
 */
UCLASS(Blueprintable, BlueprintType)
class CHRONICLESOFTHEFORGOTTEN_API UGameplayEffect : public UObject
{
	GENERATED_BODY()

public:
	virtual float ApplyToBuildCost(float BaseCost) { return BaseCost; }
	virtual int32 ApplyToGatherYield(int32 BaseYield) { return BaseYield; }
	virtual float ApplyToDamage(float BaseDamage) { return BaseDamage; }
};

/**
 * Reduces build cost by a percentage.
 */
UCLASS()
class UEffect_ModifyBuildCost : public UGameplayEffect
{
	GENERATED_BODY()
public:
	virtual float ApplyToBuildCost(float BaseCost) override { return BaseCost * 0.95f; } // 5% reduction
};

/**
 * Increases gather yield by a flat amount.
 */
UCLASS()
class UEffect_ModifyGatherYield : public UGameplayEffect
{
	GENERATED_BODY()
public:
	virtual int32 ApplyToGatherYield(int32 BaseYield) override { return BaseYield + 1; } // +1 bonus resource
};

/**
 * Increases melee damage by a percentage.
 */
UCLASS()
class UEffect_ModifyDamage : public UGameplayEffect
{
	GENERATED_BODY()
public:
	virtual float ApplyToDamage(float BaseDamage) override { return BaseDamage * 1.2f; } // 20% damage increase
};
