// Copyright Epic Games, Inc. All Rights Reserved.

#include "CotFHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "CraftingData.h"

ACotFHUD::ACotFHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/ThirdPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	// Find the crafting recipes data table
	static ConstructorHelpers::FObjectFinder<UDataTable> RecipesTableObj(TEXT("/Game/Data/DT_CraftingRecipes.DT_CraftingRecipes"));
	if (RecipesTableObj.Succeeded())
	{
		CraftingRecipesTable = RecipesTableObj.Object;
	}

	bIsCraftingMenuVisible = true; // Default to visible for debugging
}


void ACotFHUD::DrawHUD()
{
	Super::DrawHUD();

	if(bIsCraftingMenuVisible)
	{
		ShowCraftingMenu();
	}

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void ACotFHUD::ShowCraftingMenu()
{
	if (!CraftingRecipesTable) return;

	float YPos = 50.f;
	float XPos = 50.f;

	DrawText(TEXT("CRAFTING RECIPES (Press C to craft first)"), FLinearColor::White, XPos, YPos, nullptr, 1.2f);
	YPos += 25;

	for (auto it = CraftingRecipesTable->GetRowMap().CreateConstIterator(); it; ++it)
	{
		FName RowName = it.Key();
		FCraftingRecipe* Recipe = reinterpret_cast<FCraftingRecipe*>(it.Value());
		if (Recipe)
		{
			DrawText(Recipe->Description, FLinearColor::Yellow, XPos, YPos);
			YPos += 20;

			for (const FItemQuantity& Item : Recipe->RequiredItems)
			{
				FString ReqText = FString::Printf(TEXT("  - %d x %s"), Item.Quantity, *Item.ItemID.ToString());
				DrawText(ReqText, FLinearColor::Gray, XPos, YPos);
				YPos += 15;
			}
		}
	}
}

void ACotFHUD::HideCraftingMenu()
{
	bIsCraftingMenuVisible = false;
}
