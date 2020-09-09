// Copyright Samuel Carriere 2020


#include "AttackNotifyState.h"
#include "CombatAIController.h"
#include "ControllableCharacter.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AControllableCharacter* Character = Cast<AControllableCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->NotifyComboStart(TotalDuration, WidgetType);
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AControllableCharacter* Character = Cast<AControllableCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->NotifyComboEnd();
}
