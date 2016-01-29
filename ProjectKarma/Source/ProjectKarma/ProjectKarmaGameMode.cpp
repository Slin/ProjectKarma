// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ProjectKarma.h"
#include "ProjectKarmaGameMode.h"
#include "ProjectKarmaCharacter.h"

AProjectKarmaGameMode::AProjectKarmaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
