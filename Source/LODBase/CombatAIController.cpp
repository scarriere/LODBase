// Copyright Samuel Carriere 2020


#include "CombatAIController.h"
#include "BaseCharacter.h"

void ACombatAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	UE_LOG(LogTemp, Warning, TEXT("Move Completed"))

	if (CombatStep == CombatStep::MOVE_TO_TARGET && Result.IsSuccess())
	{
		Attack();
	}
	else if (CombatStep == CombatStep::MOVE_TO_START_LOCATION && Result.IsSuccess())
	{
		//SetFocalPoint(CombatCenter);
		CombatStep = CombatStep::IDLE;
		EndTurnFunc.ExecuteIfBound();
	}
}

void ACombatAIController::StartCombat(FVector CombatCenter)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Combat"))
	CombatStep = CombatStep::IDLE;
	SetFocalPoint(CombatCenter);
	InitialCombatPosition = GetPawn()->GetActorLocation();
}

void ACombatAIController::StartTurn(APawn* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("AI Turn Start for Pawn %s attacking %s"), *GetPawn()->GetName(), *Target->GetName())
	CombatStep = CombatStep::MOVE_TO_TARGET;
	SetFocus(Target);
	MoveToActor(Target, 10.f);
}

void ACombatAIController::ComboFail()
{
	FailAttack();
	//GetCharacter()->StopAnimMontage();
	//ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetCharacter());
	//float montage = BaseCharacter->Flinch();
	//CompleteAttack();
}

void ACombatAIController::CompleteAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Complete Attack"));
	CombatStep = CombatStep::MOVE_TO_START_LOCATION;
	MoveToLocation(InitialCombatPosition, .1f);
}

void ACombatAIController::Attack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Attack"));
	CompleteAttack();
}

void ACombatAIController::FailAttack_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Fail Attack"));
	CompleteAttack();
}