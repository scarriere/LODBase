// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "../InteractionActor.h"
#include "ItemAction.generated.h"

class AMovingActor;

USTRUCT()
struct FActionDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ItemNeeded;

	UPROPERTY(EditAnywhere)
	FText MissingText;
};

UCLASS()
class LODBASE_API AItemAction : public AInteractionActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FActionDefinition> ItemsNeeded;

	UPROPERTY(EditAnywhere)
	FText CompletedText;

	UPROPERTY(EditAnywhere)
	TArray<AMovingActor*> MovingActors;

public:
	virtual bool Interact(ABasePlayerController* PlayerController);
};
