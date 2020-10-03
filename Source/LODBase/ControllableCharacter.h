// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ControllableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACombatAIController;
class ACombatOrchestrator;
class ABasePlayerController;
class UCombatWidget;
class USphereComponent;

UCLASS()
class LODBASE_API AControllableCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AControllableCharacter();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACombatOrchestrator> CombatOrchestratorType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACombatAIController> CombatAIControllerType;

	UPROPERTY(VisibleAnywhere)
	ACombatAIController* CombatAIController = nullptr;

	UPROPERTY(VisibleAnywhere)
	ABasePlayerController* DefaultController = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	void StartCombat(ACombatOrchestrator* Orchestractor);
	void StopCombat();

	void EncounterEnemy(ABaseCharacter* Enemy);

	ABasePlayerController* GetDefaultController();

	USpringArmComponent* GetCameraArm();
};
