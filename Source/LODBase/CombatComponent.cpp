// Copyright Samuel Carriere 2020


#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LODBaseGameModeBase.h"
#include "Data/CombatAction.h"
#include "Components/CombatActionSlot.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	DefaultActionSlot = NewObject<UCombatActionSlot>(this);
	LeftActionSlot = NewObject<UCombatActionSlot>(this);
	CenterActionSlot = NewObject<UCombatActionSlot>(this);
	RightActionSlot = NewObject<UCombatActionSlot>(this);

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

void UCombatComponent::AddNewCombatAction(FName CombatActionKey)
{
	ALODBaseGameModeBase* GameMode = Cast<ALODBaseGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) return;

	UDataTable* CombatActionTable = GameMode->GetCombatActionDataTable();
	if (CombatActionTable == nullptr) return;

	static const FString ContextString(TEXT("UCombatComponent"));

	FCombatAction* InitialActionRef = CombatActionTable->FindRow<FCombatAction>(CombatActionKey, ContextString, false);
	if (InitialActionRef == nullptr) return;

	if (!LeftActionSlot->IsEnable())
	{
		LeftActionSlot->SetCombatAction(*InitialActionRef);
	}
	else if (!CenterActionSlot->IsEnable())
	{
		CenterActionSlot->SetCombatAction(*InitialActionRef);
	}
	else if (!RightActionSlot->IsEnable())
	{
		RightActionSlot->SetCombatAction(*InitialActionRef);
	}
}
