// Copyright Epic Games, Inc. All Rights Reserved.

#include "LostArcGameMode.h"
#include "Controller/LostArcPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALostArcGameMode::ALostArcGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALostArcPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ArcCharacter/Greystone/BP_LostArcPlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}