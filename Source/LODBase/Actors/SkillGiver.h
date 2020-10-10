// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "../InteractionActor.h"
#include "SkillGiver.generated.h"

UCLASS()
class LODBASE_API ASkillGiver : public AInteractionActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	FName CombatSkill;

	UPROPERTY(EditAnywhere)
	int CharacterIndex;

public:
	virtual bool Interact(ABasePlayerController* PlayerController);
};
