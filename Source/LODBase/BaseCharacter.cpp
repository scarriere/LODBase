// Copyright Samuel Carriere 2020


#include "BaseCharacter.h"
#include "CombatAIController.h"
#include "Animation/AnimMontage.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float ABaseCharacter::Flinch()
{
	return PlayAnimMontage(FlinchAnimation);
}

UAnimMontage * ABaseCharacter::GetFlinchAnimation()
{
	return FlinchAnimation;
}
