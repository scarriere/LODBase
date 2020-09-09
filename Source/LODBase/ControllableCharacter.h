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

	FVector MoveDirection;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACombatAIController> CombatAIControllerType;

	UPROPERTY(VisibleAnywhere)
	ACombatAIController* CombatAIController = nullptr;

	UPROPERTY(VisibleAnywhere)
	ABasePlayerController* DefaultController = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCombatWidget* CurrentAttackWidget;

	float ComboStartTime = 0.f;

public:
	AControllableCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void StartCombat(APawn* Orchestractor);
	void StopCombat();

	void NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType);
	void UpdateAttackWidget();
	void NotifyComboEnd();
};
