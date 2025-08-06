// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CotFHUD.generated.h"

class UDataTable;

/**
 *
 */
UCLASS()
class CHRONICLESOFTHEFORGOTTEN_API ACotFHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACotFHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	void ShowCraftingMenu();
	void HideCraftingMenu();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Crafting")
	UDataTable* CraftingRecipesTable;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	bool bIsCraftingMenuVisible;

};
