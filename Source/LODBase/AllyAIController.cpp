// Copyright Samuel Carriere 2020


#include "AllyAIController.h"
#include "AllyCharacter.h"

void AAllyAIController::BeginPlay()
{
	Super::BeginPlay();

	AAllyCharacter* ControlledCharacter = Cast<AAllyCharacter>(GetCharacter());
	if (ControlledCharacter == nullptr) return;

	if(ControlledCharacter->OnPlayerSide())
	{
		FollowCharacter = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}
	else
	{
		FollowCharacter = ControlledCharacter->GetAllyCharacter();
	}

	if (FollowCharacter == nullptr) return;
	FollowCharacter->AddAlly(ControlledCharacter);

	GetWorld()->GetTimerManager().SetTimer(FollowTimeHandle, this, &AAllyAIController::Follow, FollowInterval, true);
}

void AAllyAIController::Follow()
{
	if (CombatStep == CombatStep::NOT_IN_COMBAT)
	{
		MoveToActor(FollowCharacter, FollowDistance);
	}
}

void AAllyAIController::StartCombat(APawn* Target)
{
	Super::StartCombat(Target);

	GetWorld()->GetTimerManager().ClearTimer(FollowTimeHandle);
}

void AAllyAIController::StopCombat()
{
	Super::StopCombat();

	ClearFocus(EAIFocusPriority::Gameplay);
	GetWorld()->GetTimerManager().SetTimer(FollowTimeHandle, this, &AAllyAIController::Follow, FollowInterval, true);
}