// Copyright Samuel Carriere 2020


#include "BaseCharacter.h"
#include "CombatAIController.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//GetCharacterMovement()->bUseRVOAvoidance = true;
	//GetCharacterMovement()->AvoidanceWeight = .5f;
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
	else if (Damage > 0)
	{
		Flinch();
	}
	return Damage;
}

float ABaseCharacter::GetCurrentHealth()
{
	return CurrentHealth;
}

float ABaseCharacter::GetHealthPercent()
{
	return CurrentHealth / MaxHealth;
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

bool ABaseCharacter::OnPlayerSide()
{
	return bOnPlayerSide;
}

void ABaseCharacter::AddAlly(ABaseCharacter * Ally)
{
	Allies.Add(Ally);
}

TArray<ABaseCharacter*> ABaseCharacter::GetAllies()
{
	return Allies;
}

UAnimMontage * ABaseCharacter::GetFlinchAnimation()
{
	return FlinchAnimation;
}

UAnimMontage * ABaseCharacter::GetAttackAnimation()
{
	return AttackAnimation;
}

UAnimMontage * ABaseCharacter::GetHealAnimation()
{
	return HealAnimation;
}
