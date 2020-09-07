// Copyright Samuel Carriere 2020


#include "CombatAIController.h"

void ACombatAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	UE_LOG(LogTemp, Warning, TEXT("Move Completed"))

	if (CombatStep == CombatStep::MOVE_TO_TARGET && Result.IsSuccess())
	{
		CombatStep = CombatStep::MOVE_TO_START_LOCATION;
		MoveToLocation(InitialCombatPosition, .1f);
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
