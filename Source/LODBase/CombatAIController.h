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
	CombatStep CombatStep = CombatStep::NOT_IN_COMBAT;

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION(BlueprintNativeEvent)
	void Attack();

	UFUNCTION(BlueprintNativeEvent)
	void FailAttack();

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
};
