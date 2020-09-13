// Copyright Samuel Carriere 2020


#include "EnemyAttackNotify.h"
#include "BaseCharacter.h"
#include "CombatAIController.h"

void UEnemyAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) return;

	ACombatAIController* Controller = Cast<ACombatAIController>(Character->GetController());
	Controller->HitTarget();
}