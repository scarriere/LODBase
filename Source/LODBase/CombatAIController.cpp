// Copyright Samuel Carriere 2020


#include "CombatAIController.h"
#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void ACombatAIController::BeginPlay()
{
	Super::BeginPlay();

	AttackCompleteDelegate.BindUObject(this, &ACombatAIController::OnAttackComplete);
	FailAttackCompleteDelegate.BindUObject(this, &ACombatAIController::OnAttackComplete);
	HealCompleteDelegate.BindUObject(this, &ACombatAIController::OnAttackComplete);
	MagicCompleteDelegate.BindUObject(this, &ACombatAIController::OnCompleteMagic);
}

void ACombatAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnMoveCompleted - Fail to complete current movement"))
	}

	if (CombatStep == CombatStep::MOVE_TO_TARGET)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnMoveCompleted - CombatStep::MOVE_TO_TARGET"))
		Attack();
	}
	else if (CombatStep == CombatStep::MOVE_TO_START_LOCATION)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnMoveCompleted - CombatStep::MOVE_TO_START_LOCATION"))
		CombatStep = CombatStep::IDLE;
		NextCombatAction = CombatAction::NONE;
		EndTurnFunc.ExecuteIfBound();
	}
}

void ACombatAIController::StartCombat(APawn* Target, FVector CombatPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::StartCombat"))
	CombatStep = CombatStep::IDLE;
	SetFocus(Target, EAIFocusPriority::Gameplay);
	InitialCombatPosition = CombatPosition;
	NextCombatAction = CombatAction::NONE;
	//TODO: Find better solution
	GetCharacter()->GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterMesh"));
	MoveToLocation(InitialCombatPosition);
}

void ACombatAIController::StopCombat()
{
	CombatStep = CombatStep::NOT_IN_COMBAT;
	EndTurnFunc.Unbind();
	GetCharacter()->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
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

	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::StartTurn - AI Turn Start for Pawn %s attacking %s"), *GetPawn()->GetName(), *Target->GetName())
	if(NextCombatAction == CombatAction::HEAL)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::StartTurn - Healing"));
		Heal();
	}
	else if (NextCombatAction == CombatAction::MAGIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::StartTurn - Magic"));
		SetFocus(Target, EAIFocusPriority::Gameplay);
		CurrentTarget = Target;
		Magic();
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
		if (NextCombatAction == CombatAction::MAGIC)
		{
			//TODO: implement multiplier?
		}
		else
		{
			//TODO: Calculate the damage
			float Damage = 10.f;
			UGameplayStatics::ApplyDamage(CurrentTarget, Damage, this, GetCharacter(), nullptr);
		}
	}
}

void ACombatAIController::CompleteAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::CompleteAttack"));
	CombatStep = CombatStep::MOVE_TO_START_LOCATION;
	MoveToLocation(InitialCombatPosition, .1f);
}

void ACombatAIController::OnCompleteMagic(UAnimMontage* AnimeMontage, bool bInterrupted)
{
	if (bInterrupted) return;
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::CompleteMagic"));
	if (CurrentTarget)
	{
		float Damage = 40.f;
		UGameplayStatics::ApplyDamage(CurrentTarget, Damage, this, GetCharacter(), nullptr);
	}
	CompleteAttack();
}

void ACombatAIController::OnAttackComplete(UAnimMontage* AnimeMontage, bool bInterrupted)
{
	if (bInterrupted) return;
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnAttackComplete - %s"), *AnimeMontage->GetName());
	CompleteAttack();
}

void ACombatAIController::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::Attack"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ControlledCharacter->GetAttackAnimation());
	AnimInstance->Montage_SetEndDelegate(AttackCompleteDelegate, ControlledCharacter->GetAttackAnimation());
}

void ACombatAIController::FailAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::FailAttack"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ControlledCharacter->GetFlinchAnimation());
	AnimInstance->Montage_SetEndDelegate(FailAttackCompleteDelegate, ControlledCharacter->GetFlinchAnimation());
}


void ACombatAIController::Heal()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::Heal"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ControlledCharacter->GetHealAnimation());
	AnimInstance->Montage_SetEndDelegate(HealCompleteDelegate, ControlledCharacter->GetHealAnimation());
}

void ACombatAIController::Magic()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::Magic"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ControlledCharacter->GetMagicAnimation());
	AnimInstance->Montage_SetEndDelegate(MagicCompleteDelegate, ControlledCharacter->GetMagicAnimation());
}

void ACombatAIController::SetNextCombatAction(CombatAction CombatAction)
{
	NextCombatAction = CombatAction;
}

CombatAction ACombatAIController::GetNextCombatAction()
{
	return NextCombatAction;
}
