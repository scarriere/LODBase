// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatMenuWidget.generated.h"

class ABaseCharacter;

UCLASS()
class LODBASE_API UCombatMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ABaseCharacter*> Characters;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int SelectedCharacterIndex = -1;
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void OnSelectCharacter();

	void InitWidget(TArray<ABaseCharacter*> CurrentCharacters);
	void SetSelectedCharacter(int Index);
};
