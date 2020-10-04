// Copyright Samuel Carriere 2020


#include "DialogTrigger.h"
#include "Components/SphereComponent.h"
#include "BasePlayerController.h"
#include "ControllableCharacter.h"

ADialogTrigger::ADialogTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->SetSphereRadius(TriggerSphereRadius);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
}

void ADialogTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ADialogTrigger::OnTriggerOverlap);
}

void ADialogTrigger::OnTriggerOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AControllableCharacter* PlayerCharacter = Cast<AControllableCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());
	if (PlayerController == nullptr) return;

	PlayerController->StartDialogs(Dialogs);
	if (!AlwaysTrigger)
	{
		GetWorld()->DestroyActor(this);
	}
}