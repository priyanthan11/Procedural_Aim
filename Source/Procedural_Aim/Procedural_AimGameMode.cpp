// Copyright Epic Games, Inc. All Rights Reserved.

#include "Procedural_AimGameMode.h"
#include "Procedural_AimCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProcedural_AimGameMode::AProcedural_AimGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
