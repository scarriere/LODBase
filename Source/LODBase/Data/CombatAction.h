// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Animation/AnimMontage.h"
#include "CombatAction.generated.h"

USTRUCT(BlueprintType)
struct FCombatAction : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool MoveToTarget;

	UPROPERTY(EditAnywhere)
	bool TargetSelf;

	UPROPERTY(EditAnywhere)
	bool DelayDamage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Animation = nullptr;

	UPROPERTY(EditAnywhere)
	float BaseDamage;

	UPROPERTY(EditAnywhere)
	int CooldownTurns;
};