// Copyright Samuel Carriere 2020


#include "ItemAction.h"
#include "../BasePlayerController.h"
#include "../InventoryComponent.h"
#include "MovingActor.h"

bool AItemAction::Interact(ABasePlayerController* PlayerController)
{
	for (FActionDefinition Item : ItemsNeeded)
	{
		if (!PlayerController->GetInventoryComponent()->HasItem(Item.ItemNeeded))
		{
			TArray<FText> Dialogs;
			Dialogs.Add(Item.MissingText);
			PlayerController->StartDialogs(Dialogs);
			return false;
		}
	}

	if (!CompletedText.IsEmpty())
	{
		TArray<FText> Dialogs;
		Dialogs.Add(CompletedText);
		PlayerController->StartDialogs(Dialogs);
	}

	for (AMovingActor* MovingActor : MovingActors)
	{
		MovingActor->StartMovement();
	}

	GetWorld()->DestroyActor(this);
	return true;
}