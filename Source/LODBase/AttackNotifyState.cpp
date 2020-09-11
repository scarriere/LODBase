// Copyright Samuel Carriere 2020


#include "AttackNotifyState.h"
#include "CombatAIController.h"
#include "ControllableCharacter.h"
#include "BasePlayerController.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AControllableCharacter* Character = Cast<AControllableCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->GetDefaultController()->NotifyComboStart(TotalDuration, WidgetType);
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	AControllableCharacter* Character = Cast<AControllableCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	Character->GetDefaultController()->NotifyComboEnd();
}
