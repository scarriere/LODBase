// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABaseCharacter;
class UCombatWidget;
class UCombatMenuWidget;

UCLASS()
class LODBASE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	
private:
	UPROPERTY(EditAnywhere)
	bool Verbose = false;

	FVector MoveDirection;

	UPROPERTY(VisibleAnywhere)
	UCombatWidget* CurrentAttackWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCombatWidget> MenuTimerWidgetType;

	UPROPERTY(VisibleAnywhere)
	UCombatWidget* MenuTimerWidget = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCombatMenuWidget> MenuWidgetType;

	UPROPERTY(VisibleAnywhere)
	UCombatMenuWidget* MenuWidget = nullptr;

	int MenuCharacterSelected = -1;

	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<UCombatWidget>, UCombatWidget*> CombatWidgetMap;

	bool IsInMenuInterval = false;
	bool IsMenuOpen = false;
	float MenuStartTime = 0.f;

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
	void AttackUp();
	void AttackDown();
	void OpenCombatMenu();

public:
	virtual void Tick(float DeltaTime) override;

	void NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType, FName AttackAction);
	void UpdateAttackWidget();
	void NotifyComboEnd();

	void NotifyMenuStart(float MenuDuration);
	void NotifyMenuEnd();
	void UpdateMenuWidget();
};
