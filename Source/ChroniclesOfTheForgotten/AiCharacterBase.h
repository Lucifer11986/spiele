// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AiCharacterBase.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API AAiCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAiCharacterBase();

	virtual void Tick(float DeltaTime) override;

	// Overriding the TakeDamage function
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxHealth;

	// Function to be called when health drops to 0
	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TArray<struct FItemQuantity> LootTable;
};
