// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterStatsComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "GameplayEffect.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	static ConstructorHelpers::FObjectFinder<UDataTable> SkillsTableObj(TEXT("/Game/Data/DT_Skills.DT_Skills"));
	if (SkillsTableObj.Succeeded())
	{
		SkillDataTable = SkillsTableObj.Object;
	}

	Level = 1;
	ExperiencePoints = 0.f;
	ExperienceToNextLevel = 100.f;
	SkillPoints = 0;
}

void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterStatsComponent, Level);
	DOREPLIFETIME(UCharacterStatsComponent, ExperiencePoints);
	DOREPLIFETIME(UCharacterStatsComponent, ExperienceToNextLevel);
	DOREPLIFETIME(UCharacterStatsComponent, SkillPoints);
	DOREPLIFETIME(UCharacterStatsComponent, UnlockedSkillIDs);
}

void UCharacterStatsComponent::AddExperience(int32 Amount)
{
	if (Amount <= 0) return;

	ExperiencePoints += Amount;
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Gained %d XP. Total XP: %.0f / %.0f"), Amount, ExperiencePoints, ExperienceToNextLevel);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Msg);
	}

	while (ExperiencePoints >= ExperienceToNextLevel)
	{
		ExperiencePoints -= ExperienceToNextLevel;
		Level++;
		SkillPoints++;
		ExperienceToNextLevel *= 1.2f; // Increase next level's requirement by 20%

		if (GEngine)
		{
			FString LevelUpMsg = FString::Printf(TEXT("LEVEL UP! Reached Level %d! You have %d Skill Points."), Level, SkillPoints);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, LevelUpMsg);
		}
	}
}

void UCharacterStatsComponent::UnlockSkill(FName SkillID)
{
	if (!SkillDataTable) return;

	FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, TEXT(""));
	if (!SkillData)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Skill not found!"));
		return;
	}

	if (UnlockedSkillIDs.Contains(SkillID))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Skill already unlocked!"));
		return;
	}

	if (SkillPoints >= SkillData->SkillPointCost)
	{
		SkillPoints -= SkillData->SkillPointCost;
		UnlockedSkillIDs.Add(SkillID);
		if (GEngine)
		{
			FString Msg = FString::Printf(TEXT("Unlocked Skill: %s"), *SkillData->DisplayName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not enough skill points!"));
	}
}

float UCharacterStatsComponent::ApplyBuildCostEffects(float BaseCost)
{
	if (!SkillDataTable) return BaseCost;

	float ModifiedCost = BaseCost;

	for (const FName& SkillID : UnlockedSkillIDs)
	{
		FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, TEXT(""));
		if (SkillData && SkillData->EffectClass)
		{
			UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, SkillData->EffectClass);
			if (Effect)
			{
				ModifiedCost = Effect->ApplyToBuildCost(ModifiedCost);
			}
		}
	}

	return ModifiedCost;
}

int32 UCharacterStatsComponent::ApplyGatherYieldEffects(int32 BaseYield)
{
	if (!SkillDataTable) return BaseYield;

	int32 ModifiedYield = BaseYield;

	for (const FName& SkillID : UnlockedSkillIDs)
	{
		FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, TEXT(""));
		if (SkillData && SkillData->EffectClass)
		{
			UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, SkillData->EffectClass);
			if (Effect)
			{
				ModifiedYield = Effect->ApplyToGatherYield(ModifiedYield);
			}
		}
	}

	return ModifiedYield;
}

float UCharacterStatsComponent::ApplyDamageEffects(float BaseDamage)
{
	if (!SkillDataTable) return BaseDamage;

	float ModifiedDamage = BaseDamage;

	for (const FName& SkillID : UnlockedSkillIDs)
	{
		FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(SkillID, TEXT(""));
		if (SkillData && SkillData->EffectClass)
		{
			UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, SkillData->EffectClass);
			if (Effect)
			{
				ModifiedDamage = Effect->ApplyToDamage(ModifiedDamage);
			}
		}
	}

	return ModifiedDamage;
}
