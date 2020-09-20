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

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere)
	TArray<ABaseCharacter*> Allies;

protected:
	UPROPERTY(EditAnywhere)
	bool bOnPlayerSide = false;

	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	float Flinch();
	float Died();

	bool IsAlive();
	bool OnPlayerSide();

	void AddAlly(ABaseCharacter* Ally);
	TArray<ABaseCharacter*> GetAllies();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetFlinchAnimation();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAttackAnimation();
};
