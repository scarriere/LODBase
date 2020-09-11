// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ACombatAIController;

UCLASS()
class LODBASE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* FlinchAnimation;

protected:
	virtual void BeginPlay() override;

public:
	float Flinch();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetFlinchAnimation();
};
