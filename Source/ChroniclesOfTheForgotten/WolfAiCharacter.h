// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AiCharacterBase.h"
#include "WolfAiCharacter.generated.h"

/**
 *
 */
UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API AWolfAiCharacter : public AAiCharacterBase
{
	GENERATED_BODY()

public:
	AWolfAiCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AttackDamage;

};
