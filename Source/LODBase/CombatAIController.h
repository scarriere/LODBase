// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatComponent.h"
#include "CombatAIController.generated.h"

enum class CombatStep
{
	NOT_IN_COMBAT,
	IDLE,
	MOVE_TO_TARGET,
	MOVE_TO_START_LOCATION,
};

class ABaseCharacter;

UCLASS()
class LODBASE_API ACombatAIController : public AAIController
{
	GENERATED_BODY()

private:
	FVector InitialCombatPosition;
	APawn* CurrentTarget = nullptr;
	UCombatComponent* ActorCombatComponent;
	FCombatActionSlot NextCombatAction;

	ABaseCharacter* FindFirstAliveCharacter(TArray<ABaseCharacter*> Characters);

protected:
	virtual void BeginPlay() override;
	CombatStep CombatStep = CombatStep::NOT_IN_COMBAT;

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void Action();
	void FailAction();

	UFUNCTION(BlueprintCallable)
	void CompleteAction();

	virtual void StartCombat(APawn* Target, FVector CombatPosition);
	virtual void StopCombat();
	void StartTurn(TArray<ABaseCharacter*> PlayerCharacters, TArray<ABaseCharacter*> EnemyCharacters);

	void ComboFail();
	void ComboSucceed();
	void HitTarget();

	bool SetNextCombatAction(ActionSlotPosition CombatAction);

	UFUNCTION(BlueprintCallable)
	FCombatActionSlot GetNextCombatAction();

	DECLARE_DELEGATE(EndTurn)
	EndTurn EndTurnFunc;

	void OnActionComplete(UAnimMontage* AnimeMontage, bool bInterrupted);
	void OnActionFail(UAnimMontage* AnimeMontage, bool bInterrupted);

	DECLARE_DELEGATE_TwoParams(AnimationCompleteDelegate, class UAnimMontage*, bool /*bInterrupted*/)
	AnimationCompleteDelegate ActionCompleteDelegate;
	AnimationCompleteDelegate FailActionCompleteDelegate;
};
