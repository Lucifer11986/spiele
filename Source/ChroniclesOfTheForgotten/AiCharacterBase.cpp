// Copyright Epic Games, Inc. All Rights Reserved.

#include "AiCharacterBase.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LootContainerActor.h"
#include "CotFCharacter.h"
#include "CharacterStatsComponent.h"

// Sets default values
AAiCharacterBase::AAiCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000.f;

	MaxHealth = 100.0f;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void AAiCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAiCharacterBase::OnSeePawn);
	}

	// Start Behavior Tree
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && BehaviorTree)
	{
		AIController->RunBehaviorTree(BehaviorTree);
	}
}

void AAiCharacterBase::OnSeePawn(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		// For now, just log. Later, we will set this in the blackboard.
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("I see %s"), *Pawn->GetName()));

		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		if(BlackboardComp)
		{
			BlackboardComp->SetValueAsObject("TargetActor", Pawn);
		}
	}
}

float AAiCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.0f)
	{
		Health -= ActualDamage;
		if (Health <= 0.0f)
		{
			// Grant XP to the killer
			ACotFCharacter* Killer = Cast<ACotFCharacter>(DamageCauser);
			if (Killer && Killer->StatsComponent)
			{
				Killer->StatsComponent->AddExperience(ExperienceValue);
			}

			OnDeath();
		}
	}
	return ActualDamage;
}

void AAiCharacterBase::OnDeath()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("AI has died."));

	// Stop AI logic
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBrainComponent()->StopLogic("Death");
	}

	// Spawn loot container
	if (LootTable.Num() > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ALootContainerActor* LootContainer = GetWorld()->SpawnActor<ALootContainerActor>(GetActorLocation(), GetActorRotation(), SpawnParams);
		if (LootContainer)
		{
			LootContainer->LootTable = LootTable;
			// You might want to set a mesh on the container, e.g., a loot bag.
		}
	}

	// Disable collision and simulate physics for a ragdoll effect
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetCharacterMovement()->DisableMovement();

	// Set a timer to destroy the actor after some time
	SetLifeSpan(10.0f);
}


// Called every frame
void AAiCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
