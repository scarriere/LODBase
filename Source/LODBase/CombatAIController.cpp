// Copyright Samuel Carriere 2020


#include "CombatAIController.h"
#include "BaseCharacter.h"
#include "CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CombatActionSlot.h"

void ACombatAIController::BeginPlay()
{
	Super::BeginPlay();

	ActionCompleteDelegate.BindUObject(this, &ACombatAIController::OnActionComplete);
	FailActionCompleteDelegate.BindUObject(this, &ACombatAIController::OnActionFail);
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
		Action();
	}
	else if (CombatStep == CombatStep::MOVE_TO_START_LOCATION)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnMoveCompleted - CombatStep::MOVE_TO_START_LOCATION"))
		CombatStep = CombatStep::IDLE;
		NextCombatAction = ActorCombatComponent->GetCombatActionSlot(ActionSlotPosition::DEFAULT);
		EndTurnFunc.ExecuteIfBound();
	}
}

void ACombatAIController::StartCombat(APawn* Target, FVector CombatPosition)
{
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	ActorCombatComponent = ControlledCharacter->GetCombatComponent();
	if (ActorCombatComponent == nullptr) return;

	NextCombatAction = ActorCombatComponent->GetCombatActionSlot(ActionSlotPosition::DEFAULT);

	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::StartCombat"))
	CombatStep = CombatStep::IDLE;
	SetFocus(Target, EAIFocusPriority::Gameplay);
	InitialCombatPosition = CombatPosition;
	NextCombatAction = ActorCombatComponent->GetCombatActionSlot(ActionSlotPosition::DEFAULT);
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

	ControlledCharacter->GetCombatComponent()->RefreshSlots();

	ABaseCharacter* Target = ControlledCharacter->OnPlayerSide() ? FindFirstAliveCharacter(EnemyCharacters) : FindFirstAliveCharacter(PlayerCharacters);
	if (Target == nullptr) return;

	FCombatAction CombatAction = NextCombatAction->GetCombatAction();
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::StartTurn - AI Turn Start for Pawn %s attacking %s"), *GetPawn()->GetName(), *Target->GetName())
	if (CombatAction.TargetSelf)
	{
		CurrentTarget = GetCharacter();
	}
	else
	{
		CurrentTarget = Target;
		SetFocus(Target, EAIFocusPriority::Gameplay);
	}
	if (CombatAction.MoveToTarget)
	{
		CombatStep = CombatStep::MOVE_TO_TARGET;
		MoveToActor(Target, 10.f);
	}
	else
	{
		Action();
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
	FailAction();
}

void ACombatAIController::ComboSucceed()
{
	if (!NextCombatAction->GetCombatAction().DelayDamage)
	{
		HitTarget();
	}
}

void ACombatAIController::HitTarget()
{
	if (CurrentTarget)
	{
		UGameplayStatics::ApplyDamage(CurrentTarget, NextCombatAction->GetCombatAction().BaseDamage, this, GetCharacter(), nullptr);
	}
}

void ACombatAIController::CompleteAction()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::CompleteAttack"));
	CombatStep = CombatStep::MOVE_TO_START_LOCATION;
	MoveToLocation(InitialCombatPosition, .1f);
}

void ACombatAIController::OnActionComplete(UAnimMontage* AnimeMontage, bool bInterrupted)
{
	if (bInterrupted) return;
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnActionComplete - %s"), *AnimeMontage->GetName());
	FCombatAction CombatAction = NextCombatAction->GetCombatAction();
	if (CurrentTarget && CombatAction.DelayDamage)
	{
		UGameplayStatics::ApplyDamage(CurrentTarget, CombatAction.BaseDamage, this, GetCharacter(), nullptr);
	}
	CompleteAction();
}

void ACombatAIController::OnActionFail(UAnimMontage* AnimeMontage, bool bInterrupted)
{
	if (bInterrupted) return;
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::OnActionFail - %s"), *AnimeMontage->GetName());
	CompleteAction();
}

void ACombatAIController::Action()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::Attack"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	FCombatAction CombatAction = NextCombatAction->UseSlot();
	if (CombatAction.Animation == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CombatAction.Animation);
	AnimInstance->Montage_SetEndDelegate(ActionCompleteDelegate, CombatAction.Animation);
}

void ACombatAIController::FailAction()
{
	UE_LOG(LogTemp, Warning, TEXT("ACombatAIController::FailAttack"));
	ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	UAnimInstance* AnimInstance = ControlledCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ControlledCharacter->GetFlinchAnimation());
	AnimInstance->Montage_SetEndDelegate(FailActionCompleteDelegate, ControlledCharacter->GetFlinchAnimation());
}

bool ACombatAIController::SetNextCombatAction(ActionSlotPosition CombatAction)
{
	UCombatActionSlot* SelectedActionSlot = ActorCombatComponent->GetCombatActionSlot(CombatAction);
	if (SelectedActionSlot->IsReady())
	{
		NextCombatAction = SelectedActionSlot;
		return true;
	}
	return false;
}

UCombatActionSlot* ACombatAIController::GetNextCombatAction()
{
	return NextCombatAction;
}
