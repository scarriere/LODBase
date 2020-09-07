// Copyright Samuel Carriere 2020

#pragma once

#include "TurnState.generated.h"

UENUM()
enum TurnState
{
	Idle UMETA(DisplayName = "Idle"),
	ReachingTarget UMETA(DisplayName = "ReachingTarget"),
	Attacking UMETA(DisplayName = "Attacking"),
	MovingBack UMETA(DisplayName = "MovingBack"),
};
