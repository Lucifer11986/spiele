// Copyright Epic Games, Inc. All Rights Reserved.

#include "WolfAiCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"
#include "CraftingData.h" // For FItemQuantity

AWolfAiCharacter::AWolfAiCharacter()
{
    // Set Wolf-specific stats
    MaxHealth = 50.f;
    Health = MaxHealth;
    AttackDamage = 10.f;
    ExperienceValue = 20;

    // Set Wolf-specific movement
    GetCharacterMovement()->MaxWalkSpeed = 450.f;

    // Assign the Behavior Tree
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTreeObj(TEXT("/Game/AI/BT_Wolf.BT_Wolf"));
    if (BTreeObj.Succeeded())
    {
        BehaviorTree = BTreeObj.Object;
    }

    // Define loot
    LootTable.Add(FItemQuantity{ "RawMeat", 2 });
    LootTable.Add(FItemQuantity{ "Leather", 1 });
}
