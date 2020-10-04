// Copyright Samuel Carriere 2020


#include "DialogWidget.h"

void UDialogWidget::SetDialogText(FText Text)
{
	DialogText = Text;
	OnDialogChange();
}


void UDialogWidget::OnDialogChange_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Dialog Change"));
}