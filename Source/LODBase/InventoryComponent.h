// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LODBASE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

private:
	int MaxQuantity = 99;
	TMap<FString, int> InventoryMap;

protected:
	virtual void BeginPlay() override;

public:
	void AddItem(FString ItemName);
	bool HasItem(FString ItemName);
	void UseItem(FString ItemName);
};
