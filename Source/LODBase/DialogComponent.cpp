// Copyright Samuel Carriere 2020


#include "DialogComponent.h"
#include "DialogWidget.h"
#include "BaseCharacter.h"

UDialogComponent::UDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogComponent::BeginPlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (PlayerController == nullptr) return;

	DialogWidget = CreateWidget<UDialogWidget>(PlayerController, DialogWidgetType);
}

void UDialogComponent::AddDialogTexts(TArray<FText> NewDialogs)
{
	if (DialogWidget == nullptr) return;

	for (FText Dialog : NewDialogs)
	{
		TextQueue.Enqueue(Dialog);
	}
}

bool UDialogComponent::NextDialogText()
{
	if (DialogWidget == nullptr) return false;

	FText Dialog;
	if (!TextQueue.Dequeue(Dialog))
	{
		DialogWidget->RemoveFromViewport();
		return false;
	}

	if (!DialogWidget->IsInViewport())
	{
		DialogWidget->AddToViewport();
	}
	DialogWidget->SetDialogText(Dialog);
	return true;
}