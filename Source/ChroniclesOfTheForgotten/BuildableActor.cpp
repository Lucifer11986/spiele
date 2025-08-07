// Copyright Epic Games, Inc. All Rights Reserved.

#include "BuildableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"

// Sets default values
ABuildableActor::ABuildableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	bReplicates = true;
	SetReplicateMovement(true);
	Health = 100.0f;
}

void ABuildableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildableActor, MaterialID);
	DOREPLIFETIME(ABuildableActor, BuildableType);
	DOREPLIFETIME(ABuildableActor, Health);
}

// Called when the game starts or when spawned
void ABuildableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildableActor::InitializeBuildable(FName InMaterialID, EBuildableType InBuildableType, UDataTable* MaterialDataTable)
{
	MaterialID = InMaterialID;
	BuildableType = InBuildableType;

	if (MaterialDataTable)
	{
		FBuildingMaterialData* Row = MaterialDataTable->FindRow<FBuildingMaterialData>(MaterialID, TEXT(""));
		if (Row && Row->PartData.Contains(BuildableType))
		{
			const FBuildingPartInfo& PartInfo = Row->PartData[BuildableType];
			if (!PartInfo.PartMesh.IsNull())
			{
				MeshComponent->SetStaticMesh(PartInfo.PartMesh.LoadSynchronous());
			}
			Health = 100.f; // Or get from data table later
		}
	}
}
