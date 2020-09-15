// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CombatOrchestrator.generated.h"

class ABaseCharacter;
class AControllableCharacter;
class ACombatAIController;
class UCameraComponent;

UCLASS()
class LODBASE_API ACombatOrchestrator : public APawn
{
	GENERATED_BODY()
	
public:
	ACombatOrchestrator();

private:
	AControllableCharacter* PlayerCharacter = nullptr;

	TQueue<ACombatAIController*> TurnQueue;
	ACombatAIController* CurrentTurnController = nullptr;

	TArray<ABaseCharacter*> PlayerCharacters;
	TArray<ABaseCharacter*> EnemyCharacters;

	UPROPERTY(VisibleAnywhere)
	FVector CombatCenter;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CombatCamera = nullptr;

	UPROPERTY(EditAnywhere)
	float TurnWaitTime = 5.f;

	UFUNCTION()
	void EndCurrentTurn();

	bool HasOneCharacterAlive(TArray<ABaseCharacter*> Characters);
	void EndCombat(bool PlayerWon);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Initialize(AControllableCharacter* PlayerCharacter, ABaseCharacter* EnemyCharacter);
	void AddCharacter(ABaseCharacter* NewCharacter, bool bOnPlayerSide);

	ACombatAIController* GetCurrentTurnController();
};
