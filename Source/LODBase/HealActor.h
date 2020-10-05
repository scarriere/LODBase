// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "HealActor.generated.h"

UCLASS()
class LODBASE_API AHealActor : public AInteractionActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float Amount = 40.f;
	
public:
	virtual bool Interact(ABasePlayerController* PlayerController);
};
