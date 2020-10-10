// Copyright Samuel Carriere 2020


#include "EnemyCharacter.h"

#include "Components/SphereComponent.h"
#include "EnemyAIController.h"
#include "BasePlayerController.h"
#include "ControllableCharacter.h"
#include "Actors/ItemPickup.h"

AEnemyCharacter::AEnemyCharacter()
{
	VisibilitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Visibility Sphere"));
	VisibilitySphere->SetupAttachment(RootComponent);
	VisibilitySphere->SetSphereRadius(VisibilitySphereRadius);
	VisibilitySphere->SetCollisionProfileName(TEXT("Trigger"));

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	CombatSphere->SetupAttachment(RootComponent);
	CombatSphere->SetSphereRadius(CombatSphereRadius);
	CombatSphere->SetCollisionProfileName(TEXT("Trigger"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = GetController<AEnemyAIController>();
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
	AControllableCharacter* OtherCharacter =  Cast<AControllableCharacter>(OtherActor);
	if (OtherCharacter == nullptr) return;

	VisibilitySphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OtherCharacter->EncounterEnemy(this);
}

float AEnemyCharacter::Died()
{
	float AnimationTime = Super::Died();

	if (!ItemDropName.IsEmpty())
	{
		AItemPickup* ItemDrop = GetWorld()->SpawnActorDeferred<AItemPickup>(AItemPickup::StaticClass(), GetActorTransform());
		ItemDrop->SetItemName(ItemDropName);
		ItemDrop->FinishSpawning(GetActorTransform());
	}
	return AnimationTime;
}