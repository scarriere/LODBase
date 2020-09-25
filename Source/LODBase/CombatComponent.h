// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType)
enum ActionSlotPosition
{
	DEFAULT,
	LEFT,
	CENTER,
	RIGHT,
};

USTRUCT(BlueprintType)
struct FCombatActionSlot : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Enable;

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
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LODBASE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

private:
	UPROPERTY(EditAnywhere)
	FName DefaultActionSlotKey;

	UPROPERTY(EditAnywhere)
	FName LeftActionSlotKey;

	UPROPERTY(EditAnywhere)
	FName CenterActionSlotKey;

	UPROPERTY(EditAnywhere)
	FName RightActionSlotKey;

	UPROPERTY(EditAnywhere)
	FCombatActionSlot DefaultActionSlot;

	UPROPERTY(EditAnywhere)
	FCombatActionSlot LeftActionSlot;

	UPROPERTY(EditAnywhere)
	FCombatActionSlot CenterActionSlot;

	UPROPERTY(EditAnywhere)
	FCombatActionSlot RightActionSlot;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	FCombatActionSlot GetCombatActionSlot(ActionSlotPosition ActionSlotPosition);
};
