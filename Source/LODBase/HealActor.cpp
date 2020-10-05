// Copyright Samuel Carriere 2020


#include "HealActor.h"
#include "BasePlayerController.h"
#include "ControllableCharacter.h"
#include "Kismet/GameplayStatics.h"

bool AHealActor::Interact(ABasePlayerController* PlayerController)
{
	AControllableCharacter* PlayerCharacter = Cast<AControllableCharacter>(PlayerController->GetCharacter());
	if (PlayerCharacter == nullptr) return false;

	UGameplayStatics::ApplyDamage(PlayerCharacter, -Amount, nullptr, this, nullptr);
	for (ABaseCharacter* Ally : PlayerCharacter->GetAllies())
	{
		UGameplayStatics::ApplyDamage(Ally, -Amount, nullptr, this, nullptr);
	}
	GetWorld()->DestroyActor(this);
	return true;
}