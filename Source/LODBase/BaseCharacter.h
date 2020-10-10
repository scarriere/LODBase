// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ACombatAIController;
class UWidgetComponent;
class UCombatComponent;
class ABaseWeapon;

UCLASS()
class LODBASE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

private:
	UPROPERTY(EditAnywhere)
	UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere)
	UAnimMontage* FlinchAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ReviveAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere)
	TArray<ABaseCharacter*> Allies;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* DamageWidgetComponent;

	//TODO: Have inventory component
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseWeapon> WeaponType;

	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* Weapon = nullptr;

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
	virtual float Died();
	float Revive();

	bool IsAlive();
	bool OnPlayerSide();

	void AddAlly(ABaseCharacter* Ally);
	TArray<ABaseCharacter*> GetAllies();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetFlinchAnimation();

	UFUNCTION(BlueprintCallable)
	UCombatComponent* GetCombatComponent();
};
