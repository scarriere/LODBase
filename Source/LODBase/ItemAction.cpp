// Copyright Samuel Carriere 2020


#include "ItemAction.h"
#include "BasePlayerController.h"
#include "InventoryComponent.h"

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
	TArray<FText> Dialogs;
	Dialogs.Add(CompletedText);
	PlayerController->StartDialogs(Dialogs);
	GetWorld()->DestroyActor(this);
	return true;
}