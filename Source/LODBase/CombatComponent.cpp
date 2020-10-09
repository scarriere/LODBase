// Copyright Samuel Carriere 2020


#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LODBaseGameModeBase.h"
#include "Data/CombatAction.h"
#include "Components/CombatActionSlot.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultActionSlot = CreateDefaultSubobject<UCombatActionSlot>(TEXT("Default Slot"));
	LeftActionSlot = CreateDefaultSubobject<UCombatActionSlot>(TEXT("Left Slot"));
	CenterActionSlot = CreateDefaultSubobject<UCombatActionSlot>(TEXT("Center Slot"));
	RightActionSlot = CreateDefaultSubobject<UCombatActionSlot>(TEXT("Right Slot"));
}

void UCombatComponent::BeginPlay()
{
	ALODBaseGameModeBase* GameMode = Cast<ALODBaseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) return;

	UDataTable* CombatActionTable = GameMode->GetCombatActionDataTable();
	if (CombatActionTable == nullptr) return;

	static const FString ContextString(TEXT("UCombatComponent"));

	for (FActionSlotMapping InitialSlotMapping : InitialSlotMappings)
	{
		FCombatAction* InitialActionRef = CombatActionTable->FindRow<FCombatAction>(InitialSlotMapping.CombatActionKey, ContextString, false);
		if (InitialActionRef != nullptr)
		{
			GetCombatActionSlot(InitialSlotMapping.SlotPosition)->SetCombatAction(*InitialActionRef);
		}
	}
}

UCombatActionSlot* UCombatComponent::GetCombatActionSlot(ActionSlotPosition ActionSlotPosition)
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

void UCombatComponent::RefreshSlots()
{
	DefaultActionSlot->DecreaseCooldown(1);
	LeftActionSlot->DecreaseCooldown(1);
	CenterActionSlot->DecreaseCooldown(1);
	RightActionSlot->DecreaseCooldown(1);
}