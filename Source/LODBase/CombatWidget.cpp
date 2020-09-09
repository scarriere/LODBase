// Copyright Samuel Carriere 2020


#include "CombatWidget.h"

void UCombatWidget::ResetWidget(float Duration)
{
	TotalDuration = Duration;
	Completion = 0.f;
}

void UCombatWidget::UpdateElapseTime(float ElapseTime)
{
	Completion = ElapseTime / TotalDuration;
}
