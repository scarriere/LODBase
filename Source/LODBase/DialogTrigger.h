// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogTrigger.generated.h"

class USphereComponent;

UCLASS()
class LODBASE_API ADialogTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogTrigger();

private:
	float TriggerSphereRadius = 200.f;

	UPROPERTY(EditAnywhere)
	bool AlwaysTrigger = false;

	UPROPERTY(EditAnywhere)
	TArray<FText> Dialogs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* TriggerSphere;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void BeginPlay() override;
};
