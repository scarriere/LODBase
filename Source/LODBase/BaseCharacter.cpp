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

	DefaultController = GetController();
	CombatController = GetWorld()->SpawnActor<ACombatAIController>(CombatControllerType);
}

void ABaseCharacter::StartCombat(FVector CombatCenter)
{
	DefaultController->StopMovement();
	DefaultController->UnPossess();
	CombatController->Possess(this);
	CombatController->StartCombat(CombatCenter);
}

void ABaseCharacter::StopCombat()
{
	CombatController->StopMovement();
	CombatController->UnPossess();
	DefaultController->Possess(this);
}

ACombatAIController * ABaseCharacter::GetCombatController()
{
	return CombatController;
}
