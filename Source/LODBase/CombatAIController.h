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

UCLASS()
class LODBASE_API ACombatAIController : public AAIController
{
	GENERATED_BODY()

private:
	FVector InitialCombatPosition;

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

	void StartCombat(FVector CombatCenter);
	void StartTurn(APawn* Target);

	void ComboFail();

	DECLARE_DELEGATE(EndTurn)
	EndTurn EndTurnFunc;
};
