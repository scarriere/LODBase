// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAIController.generated.h"

enum class CombatStep
{
	NOT_IN_COMBAT,
	IDLE,
	MOVE_TO_TARGET,
	MOVE_TO_START_LOCATION,
};

UENUM(BlueprintType)
enum CombatAction
{
	NONE,
	HEAL,
	MAGIC,
};

class ABaseCharacter;

UCLASS()
class LODBASE_API ACombatAIController : public AAIController
{
	GENERATED_BODY()

private:
	FVector InitialCombatPosition;
	APawn* CurrentTarget = nullptr;
	CombatAction NextCombatAction = CombatAction::NONE;

	ABaseCharacter* FindFirstAliveCharacter(TArray<ABaseCharacter*> Characters);

protected:
	virtual void BeginPlay() override;
	CombatStep CombatStep = CombatStep::NOT_IN_COMBAT;

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void Attack();
	void FailAttack();
	void Heal();

	UFUNCTION(BlueprintCallable)
	void CompleteAttack();

	virtual void StartCombat(APawn* Target, FVector CombatPosition);
	virtual void StopCombat();
	void StartTurn(TArray<ABaseCharacter*> PlayerCharacters, TArray<ABaseCharacter*> EnemyCharacters);

	void ComboFail();
	void ComboSucceed();
	void HitTarget();

	void SetNextCombatAction(CombatAction CombatAction);

	UFUNCTION(BlueprintCallable)
	CombatAction GetNextCombatAction();

	DECLARE_DELEGATE(EndTurn)
	EndTurn EndTurnFunc;

	void OnAttackComplete(UAnimMontage* AnimeMontage, bool bInterrupted);

	DECLARE_DELEGATE_TwoParams(AnimationCompleteDelegate, class UAnimMontage*, bool /*bInterrupted*/)
	AnimationCompleteDelegate AttackCompleteDelegate;
	AnimationCompleteDelegate FailAttackCompleteDelegate;
	AnimationCompleteDelegate HealCompleteDelegate;
};
