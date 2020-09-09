// Copyright Samuel Carriere 2020


#include "BasePlayerController.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatOrchestrator.h"

ABasePlayerController::ABasePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetViewTarget(GetCharacter());

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &ABasePlayerController::MoveRight);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABasePlayerController::Jump);
}

void ABasePlayerController::MoveForward(float AxisValue)
{
	MoveDirection.X = AxisValue;
}

void ABasePlayerController::MoveRight(float AxisValue)
{
	MoveDirection.Y = AxisValue;
}

void ABasePlayerController::Jump()
{
	if (GetCharacter() == nullptr)
	{
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator == nullptr) return;

		Orchestrator->AttackKeyPressed();
	}
	else
	{
		GetCharacter()->Jump();
	}
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacter() == nullptr) return;

	if (MoveDirection.Size() > 0.1f)
	{
		SetControlRotation(MoveDirection.Rotation());
		GetCharacter()->AddMovementInput(GetCharacter()->GetActorForwardVector() * MoveDirection.Size());
	}
	else
	{
		SetControlRotation(GetCharacter()->GetActorRotation());
	}
}
