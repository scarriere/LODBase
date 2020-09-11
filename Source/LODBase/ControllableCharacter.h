// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ControllableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACombatAIController;
class ABasePlayerController;
class UCombatWidget;

UCLASS()
class LODBASE_API AControllableCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACombatAIController> CombatAIControllerType;

	UPROPERTY(VisibleAnywhere)
	ACombatAIController* CombatAIController = nullptr;

	UPROPERTY(VisibleAnywhere)
	ABasePlayerController* DefaultController = nullptr;

public:
	AControllableCharacter();

protected:
	virtual void BeginPlay() override;

public:
	void StartCombat(APawn* Orchestractor);
	void StopCombat();

	ABasePlayerController* GetDefaultController();
};
