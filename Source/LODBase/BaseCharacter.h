// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ACombatAIController;

UCLASS()
class LODBASE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACombatAIController> CombatControllerType;

	ACombatAIController* CombatController;

	AController* DefaultController = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	void StartCombat(FVector CombatCenter);
	void StopCombat();
	ACombatAIController* GetCombatController();
};
