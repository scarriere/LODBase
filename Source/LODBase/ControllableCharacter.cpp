// Copyright Samuel Carriere 2020


#include "ControllableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

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

	if (MoveDirection.Size() > 0.1f)
	{
		GetController()->SetControlRotation(MoveDirection.Rotation());
		AddMovementInput(GetActorForwardVector() * MoveDirection.Size());
	}
	else
	{
		GetController()->SetControlRotation(GetActorRotation());
	}
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FColor::Red, false, -1.f, 0, 5.f);
}

void AControllableCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AControllableCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AControllableCharacter::MoveRight);
}

void AControllableCharacter::MoveForward(float AxisValue)
{
	MoveDirection.X = AxisValue;
	//AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AControllableCharacter::MoveRight(float AxisValue)
{
	MoveDirection.Y = AxisValue;
	//AddMovementInput(GetActorRightVector() * AxisValue);
}
