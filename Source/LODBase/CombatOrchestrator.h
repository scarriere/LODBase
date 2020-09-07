// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CombatOrchestrator.generated.h"

class ABaseCharacter;
class UCameraComponent;

UCLASS()
class LODBASE_API ACombatOrchestrator : public APawn
{
	GENERATED_BODY()
	
public:
	ACombatOrchestrator();

private:
	ABaseCharacter* PlayerCharacter = nullptr;
	ABaseCharacter* EnemyCharacter = nullptr;

	UPROPERTY(VisibleAnywhere)
	FVector CombatCenter;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CombatCamera = nullptr;

	UFUNCTION()
	void EndCurrentTurn();

	bool IsPlayerTurn = true;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Initialize(ABaseCharacter* APlayerPawn, ABaseCharacter* AEnemyPawn);
};
