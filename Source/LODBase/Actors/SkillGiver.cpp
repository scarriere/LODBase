// Copyright Samuel Carriere 2020


#include "SkillGiver.h"
#include "../BasePlayerController.h"
#include "../BaseCharacter.h"
#include "../CombatComponent.h"

bool ASkillGiver::Interact(ABasePlayerController* PlayerController)
{
	ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(PlayerController->GetCharacter());
	if (PlayerCharacter == nullptr) return false;

	ABaseCharacter* TargetCharacter = nullptr;
	if (CharacterIndex == 0)
	{
		TargetCharacter = PlayerCharacter;
	}
	else
	{
		if (CharacterIndex > PlayerCharacter->GetAllies().Num()) return false;
		TargetCharacter = PlayerCharacter->GetAllies()[CharacterIndex - 1];
	}

	if (TargetCharacter == nullptr) return false;

	TargetCharacter->GetCombatComponent()->AddNewCombatAction(CombatSkill);
	GetWorld()->DestroyActor(this);
	return true;
}