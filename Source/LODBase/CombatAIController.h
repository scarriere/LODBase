// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAIController.generated.h"

enum class CombatStep
{
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
	CombatStep CombatStep;

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
	void StartCombat(FVector CombatCenter);

	void StartTurn(APawn* Target);

	DECLARE_DELEGATE(EndTurn)
	EndTurn EndTurnFunc;
};
