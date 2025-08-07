// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CraftingData.h" // For FItemQuantity
#include "BuildingData.generated.h"

UENUM(BlueprintType)
enum class EBuildableType : uint8
{
	Foundation,
	Wall,
	Ceiling,
	Stairs
};

USTRUCT(BlueprintType)
struct FBuildingPartInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemQuantity Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> PartMesh;
};

USTRUCT(BlueprintType)
struct FBuildingMaterialData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EBuildableType, FBuildingPartInfo> PartData;
};
