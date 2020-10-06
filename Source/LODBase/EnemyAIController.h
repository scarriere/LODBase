// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "CombatAIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class LODBASE_API AEnemyAIController : public ACombatAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

private:
	UPROPERTY(EditAnywhere)
	bool Verbose = false;

	UPROPERTY(VisibleAnywhere)
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, Category = "Wander")
	float WanderRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Wander")
	float WanderInterval = 5.f;

	FTimerHandle WanderTimeHandle;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Wander();

public:
	UFUNCTION(BlueprintCallable)
	void SeePlayer(AActor * Player);
};
