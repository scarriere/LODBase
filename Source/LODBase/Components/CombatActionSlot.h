// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Data/CombatAction.h"
#include "CombatActionSlot.generated.h"

UCLASS()
class LODBASE_API UCombatActionSlot : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	bool Enable = false;

	UPROPERTY(VisibleAnywhere)
	FCombatAction CombatAction;

	UPROPERTY(VisibleAnywhere)
	int CooldownTurnLeft = 0;

public:
	void SetCombatAction(FCombatAction CombatAction);

	UFUNCTION(BlueprintCallable)
	FCombatAction GetCombatAction();

	UFUNCTION(BlueprintCallable)
	bool IsEnable();

	UFUNCTION(BlueprintCallable)
	int GetCooldownLeft();

	FCombatAction UseSlot();
	void DecreaseCooldown(int Amount);
	void ResetCooldown();
	bool IsReady();
};
