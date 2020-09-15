// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAIController.generated.h"

enum class CombatStep
{
	NOT_IN_COMBAT UMETA(DisplayName = "NotInCombat"),
	IDLE UMETA(DisplayName = "Idle"),
	MOVE_TO_TARGET UMETA(DisplayName = "MoveToTarget"),
	MOVE_TO_START_LOCATION UMETA(DisplayName = "MoveToStartLocation"),
};

class ABaseCharacter;

UCLASS()
class LODBASE_API ACombatAIController : public AAIController
{
	GENERATED_BODY()

private:
	FVector InitialCombatPosition;
	APawn* CurrentTarget = nullptr;

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

	void StartCombat(APawn* Target);
	void StartTurn(TArray<ABaseCharacter*> PlayerCharacters, TArray<ABaseCharacter*> EnemyCharacters);

	void ComboFail();
	void ComboSucceed();
	void HitTarget();

	DECLARE_DELEGATE(EndTurn)
	EndTurn EndTurnFunc;
};
