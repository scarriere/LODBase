// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABaseCharacter;

UCLASS()
class LODBASE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	
private:
	FVector MoveDirection;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Jump();

public:
	virtual void Tick(float DeltaTime) override;
};
