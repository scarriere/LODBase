// Copyright Samuel Carriere 2020


#include "ItemPickup.h"
#include "BasePlayerController.h"
#include "InventoryComponent.h"

bool AItemPickup::Interact(ABasePlayerController* PlayerController)
{
	TArray<FText> Dialogs;
	Dialogs.Add(FText::FromString("You've picked up " + ItemName));
	PlayerController->StartDialogs(Dialogs);
	PlayerController->GetInventoryComponent()->AddItem(ItemName);
	GetWorld()->DestroyActor(this);
	return true;
}