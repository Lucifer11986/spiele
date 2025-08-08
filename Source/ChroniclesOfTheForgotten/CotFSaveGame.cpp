// Copyright Epic Games, Inc. All Rights Reserved.

#include "CotFSaveGame.h"

UCotFSaveGame::UCotFSaveGame()
{
    SaveSlotName = TEXT("DefaultSaveSlot");
    UserIndex = 0;
    WorldTimeOfDay = 0.25f; // Default start time
}
