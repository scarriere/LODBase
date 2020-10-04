// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

UCLASS()
class LODBASE_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText DialogText;

public:
	void SetDialogText(FText Text);

	UFUNCTION(BlueprintNativeEvent)
	void OnDialogChange();
};
