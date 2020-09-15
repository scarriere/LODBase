// Copyright Samuel Carriere 2020


#include "BaseCharacter.h"
#include "CombatAIController.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHealth <= 0) return 0.f;
	UE_LOG(LogTemp, Warning, TEXT("%s taking %f damage"), *GetName(), Damage)
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	if (CurrentHealth <= 0)
	{
		Died();
	}
	else
	{
		Flinch();
	}
	return Damage;
}

float ABaseCharacter::Flinch()
{
	return PlayAnimMontage(FlinchAnimation);
}

float ABaseCharacter::Died()
{
	//TODO: Add timer for the actor to get destroy
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	return PlayAnimMontage(DeathAnimation);
}

bool ABaseCharacter::IsAlive()
{
	return CurrentHealth > 0;
}

UAnimMontage * ABaseCharacter::GetFlinchAnimation()
{
	return FlinchAnimation;
}
