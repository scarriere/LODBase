// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LODBaseGameModeBase.generated.h"

class UDataTable;

UCLASS()
class LODBASE_API ALODBaseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	UDataTable* CombatActionDataTable;

public:
	UDataTable* GetCombatActionDataTable();
};
