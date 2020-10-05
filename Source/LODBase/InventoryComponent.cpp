// Copyright Samuel Carriere 2020


#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddItem(FString ItemName)
{
	int Quatity = InventoryMap.FindOrAdd(ItemName);
	Quatity = FMath::Clamp(++Quatity, 0, MaxQuantity);
	InventoryMap.Add(ItemName, Quatity);
}

bool UInventoryComponent::HasItem(FString ItemName)
{
	return InventoryMap.Contains(ItemName);
}

void UInventoryComponent::UseItem(FString ItemName)
{
	if (InventoryMap.Contains(ItemName))
	{
		int Quatity = InventoryMap.FindOrAdd(ItemName);
		Quatity = FMath::Clamp(--Quatity, 0, MaxQuantity);
		if (Quatity == 0)
		{
			InventoryMap.Remove(ItemName);
		}
		else
		{
			InventoryMap.Add(ItemName, Quatity);
		}
	}
}
