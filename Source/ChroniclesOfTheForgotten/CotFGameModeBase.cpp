// Copyright Epic Games, Inc. All Rights Reserved.


#include "CotFGameModeBase.h"
#include "CotFCharacter.h"
#include "CotFHUD.h"
#include "UObject/ConstructorHelpers.h"

ACotFGameModeBase::ACotFGameModeBase()
{
	// set default pawn class to our C++ character
	DefaultPawnClass = ACotFCharacter::StaticClass();

	// use our custom HUD class
	HUDClass = ACotFHUD::StaticClass();
}
