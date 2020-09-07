// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurnState.h"
#include "BaseAIController.generated.h"

UCLASS()
class LODBASE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

private:
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
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SeePlayer(AActor * Player);
};
