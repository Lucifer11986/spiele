// Copyright Epic Games, Inc. All Rights Reserved.

#include "CotFCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActor.h"
#include "InventoryComponent.h"
#include "CotFHUD.h"
#include "Engine/DataTable.h"

// Sets default values
ACotFCharacter::ACotFCharacter()
{
	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Interaction
	InteractionDistance = 200.0f;
	FocusedInteractable = nullptr;

	// Survival Stats Defaults
	MaxHunger = 100.0f;
	CurrentHunger = MaxHunger;
	HungerDecrementValue = 0.5f;

	MaxThirst = 100.0f;
	CurrentThirst = MaxThirst;
	ThirstDecrementValue = 1.0f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ACotFCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(SurvivalStatTimerHandle, this, &ACotFCharacter::DecaySurvivalStats, 1.0f, true);
	}

}

void ACotFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACotFCharacter, CurrentHunger);
	DOREPLIFETIME(ACotFCharacter, CurrentThirst);
}

void ACotFCharacter::DecaySurvivalStats()
{
	if (HasAuthority())
	{
		CurrentHunger = FMath::Max(0.f, CurrentHunger - HungerDecrementValue);
		CurrentThirst = FMath::Max(0.f, CurrentThirst - ThirstDecrementValue);

		// Log to screen for debugging
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hunger: %f, Thirst: %f"), CurrentHunger, CurrentThirst));
		}
	}
}

// Called every frame
void ACotFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformInteractionCheck();
}

void ACotFCharacter::PerformInteractionCheck()
{
	FVector Location;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * InteractionDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AInteractableActor* Interactable = Cast<AInteractableActor>(HitResult.GetActor());
		if (Interactable)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(0, 0, FColor::White, Interactable->InteractionPrompt);
			}
			FocusedInteractable = Interactable;
			return;
		}
	}

	FocusedInteractable = nullptr;
}

void ACotFCharacter::Interact()
{
	if (FocusedInteractable)
	{
		FocusedInteractable->OnInteract(this);
	}
}

// Called to bind functionality to input
void ACotFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACotFCharacter::Interact);
	PlayerInputComponent->BindAction("Craft", IE_Pressed, this, &ACotFCharacter::CraftFirstItem);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACotFCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACotFCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ACotFCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACotFCharacter::CraftFirstItem()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ACotFHUD* HUD = Cast<ACotFHUD>(PC->GetHUD());
		if (HUD && HUD->CraftingRecipesTable && InventoryComponent)
		{
			TArray<FName> RowNames = HUD->CraftingRecipesTable->GetRowNames();
			if (RowNames.Num() > 0)
			{
				FCraftingRecipe* Recipe = HUD->CraftingRecipesTable->FindRow<FCraftingRecipe>(RowNames[0], TEXT(""));
				if (Recipe)
				{
					InventoryComponent->CraftItem(*Recipe);
				}
			}
		}
	}
}

void ACotFCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
