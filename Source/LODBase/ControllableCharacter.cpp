// Copyright Samuel Carriere 2020


#include "ControllableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
}

void AControllableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AControllableCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
