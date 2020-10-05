// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "ItemPickup.generated.h"

UCLASS()
class LODBASE_API AItemPickup : public AInteractionActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FString ItemName;

public:
	virtual bool Interact(ABasePlayerController* PlayerController);
};
