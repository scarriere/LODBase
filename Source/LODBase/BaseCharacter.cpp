// Copyright Samuel Carriere 2020


#include "BaseCharacter.h"
#include "CombatAIController.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DamageWidget.h"
#include "CombatComponent.h"
#include "BaseWeapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage Widget"));
	DamageWidgetComponent->SetupAttachment(RootComponent);
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComponent->SetWidgetClass(UDamageWidget::StaticClass());
	DamageWidgetComponent->SetVisibility(false);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	//GetCharacterMovement()->bUseRVOAvoidance = true;
	//GetCharacterMovement()->AvoidanceWeight = .5f;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	if (WeaponType)
	{
		Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponType, GetTransform());
		Weapon->SetOwner(this);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, FName("WeaponRight"));
	}
}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHealth <= 0) return 0.f;
	UE_LOG(LogTemp, Warning, TEXT("%s taking %f damage"), *GetName(), Damage)
	UDamageWidget* DamageWidget = Cast<UDamageWidget>(DamageWidgetComponent->GetUserWidgetObject());
	DamageWidgetComponent->SetVisibility(true);
	if (DamageWidget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting widget damage"))
		DamageWidget->ShowDamage(Damage);
	}

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
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	return PlayAnimMontage(DeathAnimation);
}

float ABaseCharacter::Revive()
{
	CurrentHealth = 1;
	if (ReviveAnimation == nullptr) return 0.f;
	return PlayAnimMontage(ReviveAnimation);
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

UCombatComponent* ABaseCharacter::GetCombatComponent()
{
	return CombatComponent;
}