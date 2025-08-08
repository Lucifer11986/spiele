// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CotFCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UCharacterStatsComponent;

UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API ACotFCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	ACotFCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Survival Stats")
	float MaxHunger;

	UPROPERTY(EditDefaultsOnly, Category = "Survival Stats")
	float MaxThirst;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Survival Stats")
	float CurrentHunger;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Survival Stats")
	float CurrentThirst;

	UPROPERTY(EditDefaultsOnly, Category = "Survival Stats")
	float HungerDecrementValue;

	UPROPERTY(EditDefaultsOnly, Category = "Survival Stats")
	float ThirstDecrementValue;

	FTimerHandle SurvivalStatTimerHandle;

	void DecaySurvivalStats();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

protected:
	// Interaction
	void PerformInteractionCheck();
	void Interact();
	void CraftFirstItem();

	// Building
	void ToggleBuildMode();
	void CycleBuildable();
	void SelectMaterialWood();
	void SelectMaterialStone();
	void UpdatePreview();
	void PlaceBuildable();

	// Combat
	void PerformMeleeAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
	bool bIsInBuildMode;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	UDataTable* BuildingMaterialsTable;

	EBuildableType CurrentBuildableType;
	FName CurrentMaterialID;

	UPROPERTY()
	ABuildableActor* PreviewActor;

	class AWorldStateManager* WorldStateManagerRef;


	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionDistance;

	AInteractableActor* FocusedInteractable;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
