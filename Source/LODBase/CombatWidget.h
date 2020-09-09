// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatWidget.generated.h"

UCLASS()
class LODBASE_API UCombatWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	float TotalDuration = 1.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Completion = 0.f;
	
public:
	UFUNCTION()
	void ResetWidget(float Duration);

	UFUNCTION()
	void UpdateElapseTime(float ElapseTime);
};
