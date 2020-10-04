// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogComponent.generated.h"

class UDialogWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LODBASE_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDialogComponent();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogWidget> DialogWidgetType;

	UPROPERTY(VisibleAnywhere)
	UDialogWidget* DialogWidget = nullptr;

	TQueue<FText> TextQueue;

protected:
	virtual void BeginPlay() override;

public:
	bool NextDialogText();
	void AddDialogTexts(TArray<FText> NewDialogs);
};
