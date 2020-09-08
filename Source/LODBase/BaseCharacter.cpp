// Copyright Samuel Carriere 2020


#include "BaseCharacter.h"
#include "CombatAIController.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}
