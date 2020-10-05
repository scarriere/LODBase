// Copyright Samuel Carriere 2020


#include "InteractionActor.h"
#include "Components/SphereComponent.h"
#include "BasePlayerController.h"
#include "ControllableCharacter.h"

AInteractionActor::AInteractionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetSphereRadius(TriggerSphereRadius);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractionActor::OnTriggerOverlap);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractionActor::OnTriggerEndOverlap);
}

bool AInteractionActor::Interact(ABasePlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("Base Interact"))
	return false;
}

void AInteractionActor::OnTriggerOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AControllableCharacter* PlayerCharacter = Cast<AControllableCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());
	if (PlayerController == nullptr) return;

	PlayerController->StartInteract(this);
}

void AInteractionActor::OnTriggerEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AControllableCharacter* PlayerCharacter = Cast<AControllableCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());
	if (PlayerController == nullptr) return;

	PlayerController->StopInteract();
}
