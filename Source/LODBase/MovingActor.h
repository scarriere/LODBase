// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingActor.generated.h"

UCLASS()
class LODBASE_API AMovingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingActor();

private:
	UPROPERTY(EditAnywhere)
	float Time = 5.f;

	UPROPERTY(VisibleAnywhere)
	float MovingTime = 0.f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MovingMesh = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* TargetTransformation = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool Moving = false;

	FTransform InitialTransform;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void StartMovement();
};
