// Copyright Samuel Carriere 2020


#include "CombatAIController.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

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
		CombatStep = CombatStep::IDLE;
		EndTurnFunc.ExecuteIfBound();
	}
}

void ACombatAIController::StartCombat(APawn* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Combat"))
	CombatStep = CombatStep::IDLE;
	SetFocus(Target, EAIFocusPriority::Gameplay);
	InitialCombatPosition = GetPawn()->GetActorLocation();
	StopMovement();
}

void ACombatAIController::StartTurn(TArray<ABaseCharacter*> PlayerCharacters, TArray<ABaseCharacter*> EnemyCharacters)
{
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr || !ControlledCharacter->IsAlive())
	{
		EndTurnFunc.ExecuteIfBound();
		return;
	}

	ABaseCharacter* Target = ControlledCharacter->OnPlayerSide() ? FindFirstAliveCharacter(EnemyCharacters) : FindFirstAliveCharacter(PlayerCharacters);
	if (Target == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("AI Turn Start for Pawn %s attacking %s"), *GetPawn()->GetName(), *Target->GetName())
	CombatStep = CombatStep::MOVE_TO_TARGET;
	SetFocus(Target, EAIFocusPriority::Gameplay);
	MoveToActor(Target, 10.f);
	CurrentTarget = Target;
}

ABaseCharacter* ACombatAIController::FindFirstAliveCharacter(TArray<ABaseCharacter*> TargetCharacters)
{
	for (ABaseCharacter* TargetCharacter : TargetCharacters)
	{
		if (TargetCharacter->IsAlive())
		{
			return TargetCharacter;
		}
	}
	return nullptr;
}

void ACombatAIController::ComboFail()
{
	FailAttack();
}

void ACombatAIController::ComboSucceed()
{
	HitTarget();
}

void ACombatAIController::HitTarget()
{
	if (CurrentTarget)
	{
		//TODO: Calculate the damage
		float Damage = 10.f;
		UGameplayStatics::ApplyDamage(CurrentTarget, Damage, this, GetCharacter(), nullptr);
	}
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