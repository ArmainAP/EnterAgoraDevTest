// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnterAgoraDevTestGameMode.h"
#include "EnterAgoraDevTestPawn.h"

AEnterAgoraDevTestGameMode::AEnterAgoraDevTestGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AEnterAgoraDevTestPawn::StaticClass();
}

