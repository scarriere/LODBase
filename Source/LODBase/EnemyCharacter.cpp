// Copyright Samuel Carriere 2020


#include "EnemyCharacter.h"

#include "Components/SphereComponent.h"
#include "ControllableCharacter.h"
#include "BaseAIController.h"

AEnemyCharacter::AEnemyCharacter()
{
	VisibilitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Visibility Sphere"));
	VisibilitySphere->SetupAttachment(RootComponent);
	VisibilitySphere->SetSphereRadius(500);
	VisibilitySphere->SetCollisionProfileName(TEXT("Trigger"));

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	CombatSphere->SetupAttachment(RootComponent);
	CombatSphere->SetSphereRadius(200);
	CombatSphere->SetCollisionProfileName(TEXT("Trigger"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = GetController<ABaseAIController>();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy character needs a BaseAIController"))
	}

	VisibilitySphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnVisibilityOverlap);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnCombatOverlap);
}

void AEnemyCharacter::OnVisibilityOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AIController == nullptr) return;
	if (Cast<AControllableCharacter>(OtherActor))
	{
		AIController->SeePlayer(OtherActor);
	}
}

void AEnemyCharacter::OnCombatOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AIController == nullptr) return;
	if (Cast<AControllableCharacter>(OtherActor))
	{
		AIController->StartCombat();
	}
}