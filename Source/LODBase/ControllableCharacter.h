// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ControllableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class LODBASE_API AControllableCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

public:
	AControllableCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
