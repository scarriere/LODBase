// Copyright Samuel Carriere 2020


#include "CombatMenuWidget.h"

void UCombatMenuWidget::InitWidget(TArray<ABaseCharacter*> CurrentCharacters)
{
	Characters = CurrentCharacters;
}

void UCombatMenuWidget::OnSelectCharacter_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Attack"));
}

void UCombatMenuWidget::SetSelectedCharacter(int Index)
{
	SelectedCharacterIndex = Index;
	OnSelectCharacter();
}