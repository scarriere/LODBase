// Copyright Samuel Carriere 2020


#include "AttackNotifyState.h"
#include "BasePlayerController.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp->GetWorld() == nullptr) return;

	ABasePlayerController* PlayerController = MeshComp->GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	if (PlayerController == nullptr) return;

	PlayerController->NotifyComboStart(TotalDuration, WidgetType, AttackAction);
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (GetWorld() == nullptr) return;

	ABasePlayerController* PlayerController = MeshComp->GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	if (PlayerController == nullptr) return;

	PlayerController->NotifyComboEnd();
}
