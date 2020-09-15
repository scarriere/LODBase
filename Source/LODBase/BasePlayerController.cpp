// Copyright Samuel Carriere 2020


#include "BasePlayerController.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatOrchestrator.h"
#include "CombatWidget.h"
#include "CombatAIController.h"

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
	InputComponent->BindAction(TEXT("AttackRight"), IE_Pressed, this, &ABasePlayerController::AttackRight);
	InputComponent->BindAction(TEXT("AttackLeft"), IE_Pressed, this, &ABasePlayerController::AttackLeft);
}

void ABasePlayerController::MoveForward(float AxisValue)
{
	MoveDirection.X = AxisValue;
}

void ABasePlayerController::MoveRight(float AxisValue)
{
	MoveDirection.Y = AxisValue;
}

void ABasePlayerController::AttackRight()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackRight()"))
	if (IsInCombo)
	{
		AttackKeyPressed(TEXT("AttackRight"));
	}
}

void ABasePlayerController::AttackLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackLeft()"))
	if (IsInCombo)
	{
		AttackKeyPressed(TEXT("AttackLeft"));
	}
}

void ABasePlayerController::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump()"))
	if (GetCharacter() != nullptr)
	{
		GetCharacter()->Jump();
	}
}

void ABasePlayerController::AttackKeyPressed(FName AttackAction)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackKeyPressed()"))
	IsInCombo = false;
	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->RemoveFromViewport();
	}

	if (ComboAttackAction == AttackAction && (GetWorld()->GetRealTimeSeconds() - ComboStartTime) / ComboDuration > ComboPrecision)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo success"))
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			Orchestrator->GetCurrentTurnController()->ComboSucceed();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Failed"))
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			Orchestrator->GetCurrentTurnController()->ComboFail();
		}
	}
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInCombo)
	{
		UpdateAttackWidget();
	}

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

void ABasePlayerController::NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType, FName AttackAction)
{
	UE_LOG(LogTemp, Warning, TEXT("NotifyComboStart()"))
	IsInCombo = true;
	ComboDuration = TotalDuration;
	ComboStartTime = GetWorld()->GetRealTimeSeconds();
	ComboAttackAction = AttackAction;

	if (CombatWidgetMap.Contains(WidgetType))
	{
		CurrentAttackWidget = CombatWidgetMap[WidgetType];
	}
	else
	{
		CurrentAttackWidget = CombatWidgetMap.Add(WidgetType, CreateWidget<UCombatWidget>(this, WidgetType));
	}

	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->ResetWidget(TotalDuration);
		CurrentAttackWidget->AddToViewport();
	}
}

void ABasePlayerController::UpdateAttackWidget()
{
	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->UpdateElapseTime(GetWorld()->GetRealTimeSeconds() - ComboStartTime);
	}
}

void ABasePlayerController::NotifyComboEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("NotifyComboEnd()"))
	if (IsInCombo)
	{
		IsInCombo = false;
		if (CurrentAttackWidget)
		{
			CurrentAttackWidget->RemoveFromViewport();
		}
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			Orchestrator->GetCurrentTurnController()->ComboFail();
		}
	}
}
