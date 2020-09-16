// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AllyCharacter.generated.h"

UCLASS()
class LODBASE_API AAllyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	ABaseCharacter* AllyCharacter = nullptr;

public:
	ABaseCharacter* GetAllyCharacter();
};
