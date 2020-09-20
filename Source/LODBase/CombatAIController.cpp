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
		NextCombatAction = CombatAction::NONE;
		EndTurnFunc.ExecuteIfBound();
	}
}

void ACombatAIController::StartCombat(APawn* Target, FVector CombatPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("Start Combat"))
	CombatStep = CombatStep::IDLE;
	SetFocus(Target, EAIFocusPriority::Gameplay);
	InitialCombatPosition = CombatPosition;
	MoveToLocation(InitialCombatPosition);
}

void ACombatAIController::StopCombat()
{
	CombatStep = CombatStep::NOT_IN_COMBAT;
	EndTurnFunc.Unbind();
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
	if(NextCombatAction == CombatAction::HEAL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Healing"));
		Heal();
	}
	else
	{
		CombatStep = CombatStep::MOVE_TO_TARGET;
		SetFocus(Target, EAIFocusPriority::Gameplay);
		MoveToActor(Target, 10.f);
		CurrentTarget = Target;
	}
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
	if (NextCombatAction == CombatAction::HEAL)
	{
		float Heal = -40.f;
		UGameplayStatics::ApplyDamage(GetCharacter(), Heal, this, GetCharacter(), nullptr);
	}
	else if (CurrentTarget)
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

void ACombatAIController::OnAttackComplete(UAnimMontage* AnimeMontage, bool bInterrupted)
{
	CompleteAttack();
}

void ACombatAIController::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Attack"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(ControlledCharacter->GetAttackAnimation());

	AttackCompleteDelegate.BindUObject(this, &ACombatAIController::OnAttackComplete);
	AnimInstance->Montage_SetEndDelegate(AttackCompleteDelegate, ControlledCharacter->GetAttackAnimation());
}

void ACombatAIController::FailAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Fail Attack"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(ControlledCharacter->GetFlinchAnimation());

	FailAttackCompleteDelegate.BindUObject(this, &ACombatAIController::OnAttackComplete);
	AnimInstance->Montage_SetEndDelegate(FailAttackCompleteDelegate, ControlledCharacter->GetFlinchAnimation());
}


void ACombatAIController::Heal()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Heal"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(ControlledCharacter->GetHealAnimation());

	HealCompleteDelegate.BindUObject(this, &ACombatAIController::OnAttackComplete);
	AnimInstance->Montage_SetEndDelegate(HealCompleteDelegate, ControlledCharacter->GetHealAnimation());
}

void ACombatAIController::SetNextCombatAction(CombatAction CombatAction)
{
	NextCombatAction = CombatAction;
}

CombatAction ACombatAIController::GetNextCombatAction()
{
	return NextCombatAction;
}
