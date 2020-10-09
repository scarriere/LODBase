// Copyright Samuel Carriere 2020


#include "CombatActionSlot.h"

void UCombatActionSlot::SetCombatAction(FCombatAction NewCombatAction)
{
	Enable = true;
	CooldownTurnLeft = 0;
	CombatAction = NewCombatAction;
}

FCombatAction UCombatActionSlot::GetCombatAction()
{
	return CombatAction;
}

FCombatAction UCombatActionSlot::UseSlot()
{
	CooldownTurnLeft = CombatAction.CooldownTurns;
	return CombatAction;
}

void UCombatActionSlot::DecreaseCooldown(int Amount)
{
	if (!Enable) return;
	CooldownTurnLeft = FMath::Max(0, CooldownTurnLeft - Amount);
}

bool UCombatActionSlot::IsReady()
{
	return Enable && CooldownTurnLeft == 0;
}


bool UCombatActionSlot::IsEnable()
{
	return Enable;
}

int UCombatActionSlot::GetCooldownLeft()
{
	return CooldownTurnLeft;
}