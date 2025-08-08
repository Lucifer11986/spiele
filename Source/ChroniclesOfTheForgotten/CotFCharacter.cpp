// Copyright Epic Games, Inc. All Rights Reserved.

#include "CotFCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActor.h"
#include "InventoryComponent.h"
#include "CharacterStatsComponent.h"
#include "CotFHUD.h"
#include "Engine/DataTable.h"
#include "BuildableActor.h"
#include "WorldStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "CotFSaveGame.h"

// Sets default values
ACotFCharacter::ACotFCharacter()
{
	SaveSlotName = "DefaultSaveSlot";

	// Create stats component
	StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));

	// Building
	bIsInBuildMode = false;
	static ConstructorHelpers::FObjectFinder<UDataTable> BuildingTableObj(TEXT("/Game/Data/DT_BuildingMaterials.DT_BuildingMaterials"));
	if (BuildingTableObj.Succeeded())
	{
		BuildingMaterialsTable = BuildingTableObj.Object;
	}
	CurrentBuildableType = EBuildableType::Foundation;
	CurrentMaterialID = "Wood"; // Default material

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

	// Find the World State Manager
	WorldStateManagerRef = Cast<AWorldStateManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWorldStateManager::StaticClass()));

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
		float HungerModifier = 1.0f;
		if (WorldStateManagerRef)
		{
			// It's night time if time is past sunset (0.75) or before sunrise (0.25)
			if (WorldStateManagerRef->CurrentTimeOfDay > 0.75f || WorldStateManagerRef->CurrentTimeOfDay < 0.25f)
			{
				HungerModifier = 1.5f; // 50% more hunger decay at night (simulating cold)
			}
		}

		CurrentHunger = FMath::Max(0.f, CurrentHunger - (HungerDecrementValue * HungerModifier));
		CurrentThirst = FMath::Max(0.f, CurrentThirst - ThirstDecrementValue);

		// Log to screen for debugging
		if (GEngine)
		{
			FString NightString = (HungerModifier > 1.0f) ? " (Night)" : "";
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hunger: %.1f, Thirst: %.1f%s"), CurrentHunger, CurrentThirst, *NightString));
		}
	}
}

// Called every frame
void ACotFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformInteractionCheck();

	if (bIsInBuildMode && PreviewActor)
	{
		UpdatePreview();
	}
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

	// Building Actions
	PlayerInputComponent->BindAction("ToggleBuildMode", IE_Pressed, this, &ACotFCharacter::ToggleBuildMode);
	PlayerInputComponent->BindAction("PlaceBuildable", IE_Pressed, this, &ACotFCharacter::PlaceBuildable);
	PlayerInputComponent->BindAction("CycleBuildable", IE_Pressed, this, &ACotFCharacter::CycleBuildable);
	PlayerInputComponent->BindAction("SelectMaterialWood", IE_Pressed, this, &ACotFCharacter::SelectMaterialWood);
	PlayerInputComponent->BindAction("SelectMaterialStone", IE_Pressed, this, &ACotFCharacter::SelectMaterialStone);

	// Save/Load
	PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &ACotFCharacter::SaveGame);
	PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &ACotFCharacter::LoadGame);


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

void ACotFCharacter::ToggleBuildMode()
{
	bIsInBuildMode = !bIsInBuildMode;

	if (bIsInBuildMode)
	{
		if (!PreviewActor)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			PreviewActor = GetWorld()->SpawnActor<ABuildableActor>(ABuildableActor::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
		}
		// Additional logic to show preview
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Build Mode Activated"));
	}
	else
	{
		if (PreviewActor)
		{
			PreviewActor->Destroy();
			PreviewActor = nullptr;
		}
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Build Mode Deactivated"));
	}
}

void ACotFCharacter::UpdatePreview()
{
	if (!PreviewActor || !BuildingMaterialsTable) return;

	// Update mesh based on current selection
	PreviewActor->InitializeBuildable(CurrentMaterialID, CurrentBuildableType, BuildingMaterialsTable);

	// Calculate position in front of player
	FVector Location;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Location, Rotation);
	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * 500.f); // 5m preview distance

	// Snap to grid
	FVector SnappedLocation = End.GridSnap(100.f);
	PreviewActor->SetActorLocation(SnappedLocation);
	PreviewActor->SetActorRotation(FRotator(0, GetControlRotation().Yaw, 0).GridSnap(FRotator(0, 45, 0)));


	// Check for valid placement (simplified)
	// In a real game, you would check for collisions here and change material color
	if (GEngine)
	{
		FString msg = FString::Printf(TEXT("Previewing %s %s"), *CurrentMaterialID.ToString(), *UEnum::GetValueAsString(CurrentBuildableType));
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, msg);
	}
}

void ACotFCharacter::CycleBuildable()
{
	if (!bIsInBuildMode) return;

	CurrentBuildableType = static_cast<EBuildableType>((static_cast<int>(CurrentBuildableType) + 1) % 4); // 4 is number of items in EBuildableType
	UpdatePreview();
}

void ACotFCharacter::SelectMaterialWood()
{
	if (!bIsInBuildMode) return;
	CurrentMaterialID = "Wood";
	UpdatePreview();
}

void ACotFCharacter::SelectMaterialStone()
{
	if (!bIsInBuildMode) return;
	CurrentMaterialID = "Stone";
	UpdatePreview();
}

void ACotFCharacter::PlaceBuildable()
{
	if (bIsInBuildMode && PreviewActor && BuildingMaterialsTable && InventoryComponent)
	{
		// (Implementation remains the same)
	}
	else
	{
		PerformMeleeAttack();
	}
}

void ACotFCharacter::PerformMeleeAttack()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (FollowCamera->GetForwardVector() * 200.f); // 2m attack range

	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f); // 50cm radius
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params))
	{
		AAiCharacterBase* AiChar = Cast<AAiCharacterBase>(HitResult.GetActor());
		if (AiChar)
		{
			float Damage = 25.0f; // Base damage
			FPointDamageEvent DamageEvent(Damage, HitResult, GetActorForwardVector(), nullptr);
			AiChar->TakeDamage(Damage, DamageEvent, GetController(), this);

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Hit %s!"), *AiChar->GetName()));
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Attack missed."));
	}
}

void ACotFCharacter::SaveGame()
{
	UCotFSaveGame* SaveGameInstance = Cast<UCotFSaveGame>(UGameplayStatics::CreateSaveGameObject(UCotFSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		// Save Player Data
		SaveGameInstance->PlayerData.Location = GetActorLocation();
		SaveGameInstance->PlayerData.Rotation = GetActorRotation();
		if(InventoryComponent)
		{
			SaveGameInstance->PlayerData.Inventory = InventoryComponent->GetInventoryContents();
		}
		if(StatsComponent)
		{
			SaveGameInstance->PlayerData.Level = StatsComponent->Level;
			SaveGameInstance->PlayerData.ExperiencePoints = StatsComponent->ExperiencePoints;
			SaveGameInstance->PlayerData.UnlockedSkillIDs = StatsComponent->UnlockedSkillIDs;
		}

		// Save World Data
		if(WorldStateManagerRef)
		{
			SaveGameInstance->WorldTimeOfDay = WorldStateManagerRef->CurrentTimeOfDay;
		}

		// Save Buildables
		TArray<AActor*> BuildableActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildableActor::StaticClass(), BuildableActors);
		for (AActor* Actor : BuildableActors)
		{
			ABuildableActor* Buildable = Cast<ABuildableActor>(Actor);
			if (Buildable)
			{
				FBuildableSaveData Data;
				Data.Transform = Buildable->GetActorTransform();
				Data.BuildableType = Buildable->BuildableType;
				Data.MaterialID = Buildable->MaterialID;
				SaveGameInstance->SavedBuildables.Add(Data);
			}
		}

		// Write to disk
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved!"));
		}
	}
}

void ACotFCharacter::LoadGame()
{
	UCotFSaveGame* LoadGameInstance = Cast<UCotFSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (LoadGameInstance)
	{
		// Load Player Data
		SetActorLocation(LoadGameInstance->PlayerData.Location);
		SetActorRotation(LoadGameInstance->PlayerData.Rotation);
		if(InventoryComponent)
		{
			InventoryComponent->Inventory = LoadGameInstance->PlayerData.Inventory;
		}
		if(StatsComponent)
		{
			StatsComponent->Level = LoadGameInstance->PlayerData.Level;
			StatsComponent->ExperiencePoints = LoadGameInstance->PlayerData.ExperiencePoints;
			StatsComponent->UnlockedSkillIDs = LoadGameInstance->PlayerData.UnlockedSkillIDs;
			// Recalculate derived stats if necessary
		}

		// Load World Data
		if(WorldStateManagerRef)
		{
			WorldStateManagerRef->CurrentTimeOfDay = LoadGameInstance->WorldTimeOfDay;
		}

		// Destroy existing buildables before loading new ones
		TArray<AActor*> OldBuildables;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildableActor::StaticClass(), OldBuildables);
		for (AActor* Actor : OldBuildables)
		{
			Actor->Destroy();
		}

		// Load Buildables
		for (const FBuildableSaveData& BuildableData : LoadGameInstance->SavedBuildables)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ABuildableActor* NewBuildable = GetWorld()->SpawnActor<ABuildableActor>(
				ABuildableActor::StaticClass(),
				BuildableData.Transform.GetLocation(),
				BuildableData.Transform.GetRotation().Rotator(),
				SpawnParams);

			if (NewBuildable)
			{
				NewBuildable->SetActorTransform(BuildableData.Transform);
				NewBuildable->InitializeBuildable(BuildableData.MaterialID, BuildableData.BuildableType, BuildingMaterialsTable);
			}
		}

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Loaded!"));
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Save Game Found."));
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
