// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CraftingData.generated.h"

USTRUCT(BlueprintType)
struct FItemQuantity
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity;
};

USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemQuantity> RequiredItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemQuantity OutputItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
};
