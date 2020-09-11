// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABaseCharacter;
class UCombatWidget;

UCLASS()
class LODBASE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	
private:
	FVector MoveDirection;

	UPROPERTY(VisibleAnywhere)
	UCombatWidget* CurrentAttackWidget;

	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<UCombatWidget>, UCombatWidget*> CombatWidgetMap;

	bool IsInCombo = false;
	float ComboPrecision = .8f;
	float ComboDuration = 0.f;
	float ComboStartTime = 0.f;

	void AttackKeyPressed();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Jump();
	void AttackRight();

public:
	virtual void Tick(float DeltaTime) override;

	void NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType);
	void UpdateAttackWidget();
	void NotifyComboEnd();
};
