// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "CombatAIController.h"
#include "AllyAIController.generated.h"

class ABaseCharacter;

UCLASS()
class LODBASE_API AAllyAIController : public ACombatAIController
{
	GENERATED_BODY()

private:
	FTimerHandle FollowTimeHandle;

	ABaseCharacter* FollowCharacter = nullptr;
	
	UPROPERTY(EditAnywhere)
	float FollowDistance = 100.f;

	UPROPERTY(EditAnywhere)
	float FollowInterval = 1.f;

protected:
	virtual void BeginPlay() override;
	virtual void StartCombat(APawn* Target, FVector CombatPosition) override;
	virtual void StopCombat() override;

	UFUNCTION(BlueprintCallable)
	void Follow();
};
