// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UCombatActionSlot;

UENUM(BlueprintType)
enum class ActionSlotPosition : uint8
{
	DEFAULT,
	LEFT,
	CENTER,
	RIGHT,
};

USTRUCT(BlueprintType)
struct FActionSlotMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ActionSlotPosition SlotPosition;

	UPROPERTY(EditAnywhere)
	FName CombatActionKey;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LODBASE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

private:
	UPROPERTY(EditAnywhere)
	TArray<FActionSlotMapping> InitialSlotMappings;

	UPROPERTY(VisibleAnywhere)
	UCombatActionSlot* DefaultActionSlot;

	UPROPERTY(VisibleAnywhere)
	UCombatActionSlot* LeftActionSlot;

	UPROPERTY(VisibleAnywhere)
	UCombatActionSlot* CenterActionSlot;

	UPROPERTY(VisibleAnywhere)
	UCombatActionSlot* RightActionSlot;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	UCombatActionSlot* GetCombatActionSlot(ActionSlotPosition ActionSlotPosition);

	void RefreshSlots();

	void AddNewCombatAction(FName CombatActionKey);
};
