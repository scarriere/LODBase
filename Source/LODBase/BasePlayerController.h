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
	float ComboPrecision = .7f;
	float ComboDuration = 0.f;
	float ComboStartTime = 0.f;
	FName ComboAttackAction;

	void AttackKeyPressed(FName AttackAction);

protected:
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Jump();
	void AttackRight();
	void AttackLeft();

public:
	virtual void Tick(float DeltaTime) override;

	void NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType, FName AttackAction);
	void UpdateAttackWidget();
	void NotifyComboEnd();
};
