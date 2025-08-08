// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillData.h"
#include "CharacterStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHRONICLESOFTHEFORGOTTEN_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatsComponent();

	void AddExperience(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void UnlockSkill(FName SkillID);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Leveling")
	int32 Level;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Leveling")
	float ExperiencePoints;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Leveling")
	float ExperienceToNextLevel;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Leveling")
	int32 SkillPoints;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Skills")
	TArray<FName> UnlockedSkillIDs;

	UPROPERTY(EditDefaultsOnly, Category = "Skills")
	UDataTable* SkillDataTable;
};
