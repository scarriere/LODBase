// Copyright Samuel Carriere 2020


#include "ControllableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "CombatAIController.h"
#include "BasePlayerController.h"

AControllableCharacter::AControllableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
}

void AControllableCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultController = Cast<ABasePlayerController>(GetController());
	CombatAIController = GetWorld()->SpawnActor<ACombatAIController>();
}


void AControllableCharacter::StartCombat()
{
	DefaultController->UnPossess();
	DefaultController->SetViewTarget(this);
	CombatAIController->Possess(this);
}

void AControllableCharacter::StopCombat()
{
	CombatAIController->UnPossess();
	DefaultController->Possess(this);
}