// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatOrchestrator.generated.h"

class ABaseCharacter;

UCLASS()
class LODBASE_API ACombatOrchestrator : public AActor
{
	GENERATED_BODY()
	
public:
	ACombatOrchestrator();

private:
	ABaseCharacter* PlayerCharacter = nullptr;
	ABaseCharacter* EnemyCharacter = nullptr;

	UPROPERTY(VisibleAnywhere)
	FVector CombatCenter;

	UFUNCTION()
	void EndCurrentTurn();

	bool IsPlayerTurn = true;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Initialize(ABaseCharacter* APlayerPawn, ABaseCharacter* AEnemyPawn);
};
