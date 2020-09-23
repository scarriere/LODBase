// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CombatOrchestrator.generated.h"

class ABaseCharacter;
class AControllableCharacter;
class ACombatAIController;
class UCameraComponent;
class USceneComponent;

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
	float MenuTimeWindow = 1.f;
	FTimerHandle MenuTimeHandle;

	UPROPERTY(EditAnywhere)
	float CombatPositionDegreeOffset = 25.f;

	FVector PlayerDirection;
	FVector EnemyDirection;

	UPROPERTY(EditAnywhere)
	float CombatRadius = 300.f;

	UFUNCTION()
	void EndCurrentTurn();
	void StartNextTurn();
	void StartMenuInterval();

	bool HasOneCharacterAlive(TArray<ABaseCharacter*> Characters);
	FVector FindCombatPosition(FVector InitialDirection, int CharacterIndex);
	FVector CalculateCombatDirection(FVector CharacterPosition);
	void EndCombat(bool PlayerWon);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Initialize(AControllableCharacter* PlayerCharacter, ABaseCharacter* EnemyCharacter);
	void AddCharacter(ABaseCharacter* NewCharacter, bool bOnPlayerSide);

	ACombatAIController* GetCurrentTurnController();
	void OpenCombatMenu();
	void CloseCombatMenu();

	TArray<ABaseCharacter*> GetPlayerCharacters();
};
