// Copyright Samuel Carriere 2020


#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LODBaseGameModeBase.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	ALODBaseGameModeBase* GameMode = Cast<ALODBaseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) return;

	UDataTable* ActionSlotTable = GameMode->GetActionSlotDataTable();
	if (ActionSlotTable == nullptr) return;

	static const FString ContextString(TEXT("UCombatComponent"));
	FCombatActionSlot* DefaultActionSlotRef = ActionSlotTable->FindRow<FCombatActionSlot>(DefaultActionSlotKey, ContextString, false);
	if (DefaultActionSlotRef != nullptr)
	{
		DefaultActionSlot = *DefaultActionSlotRef;
	}
	FCombatActionSlot* LeftActionSlotRef = ActionSlotTable->FindRow<FCombatActionSlot>(LeftActionSlotKey, ContextString, false);
	if (LeftActionSlotRef != nullptr)
	{
		LeftActionSlot = *LeftActionSlotRef;
	}
	FCombatActionSlot* RightActionSlotRef = ActionSlotTable->FindRow<FCombatActionSlot>(RightActionSlotKey, ContextString, false);
	if (RightActionSlotRef != nullptr)
	{
		RightActionSlot = *RightActionSlotRef;
	}
	FCombatActionSlot* CenterActionSlotRef = ActionSlotTable->FindRow<FCombatActionSlot>(CenterActionSlotKey, ContextString, false);
	if (CenterActionSlotRef != nullptr)
	{
		CenterActionSlot = *CenterActionSlotRef;
	}
}

FCombatActionSlot UCombatComponent::GetCombatActionSlot(ActionSlotPosition ActionSlotPosition)
{
	switch (ActionSlotPosition)
	{
		case ActionSlotPosition::LEFT:
			return LeftActionSlot;
		case ActionSlotPosition::CENTER:
			return CenterActionSlot;
		case ActionSlotPosition::RIGHT:
			return RightActionSlot;
		default:
			return DefaultActionSlot;
	}
}
