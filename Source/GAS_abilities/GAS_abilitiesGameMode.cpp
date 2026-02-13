// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_abilitiesGameMode.h"
#include "GAS_abilitiesPlayerController.h"
#include "GAS_abilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_abilitiesGameMode::AGAS_abilitiesGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGAS_abilitiesPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}