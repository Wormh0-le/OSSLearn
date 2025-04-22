// Copyright Epic Games, Inc. All Rights Reserved.

#include "OSSLearnGameMode.h"
#include "OSSLearnCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOSSLearnGameMode::AOSSLearnGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
