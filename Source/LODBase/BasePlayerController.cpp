// Copyright Samuel Carriere 2020


#include "BasePlayerController.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "BaseCharacter.h"
#include "ControllableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatOrchestrator.h"
#include "CombatWidget.h"
#include "CombatMenuWidget.h"
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
	InputComponent->BindAction(TEXT("AttackUp"), IE_Pressed, this, &ABasePlayerController::AttackUp);
	InputComponent->BindAction(TEXT("AttackDown"), IE_Pressed, this, &ABasePlayerController::AttackDown);
	InputComponent->BindAction(TEXT("CombatMenu"), IE_Pressed, this, &ABasePlayerController::OpenCombatMenu);
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
	if (IsInCombo)
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("AttackRight()"))
		AttackKeyPressed(TEXT("AttackRight"));
	}
	if (IsMenuOpen)
	{
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			if (MenuCharacterSelected == -1)
			{
				if (Orchestrator->GetPlayerCharacters().Num() >= 3)
				{
					MenuCharacterSelected = 2;
					MenuWidget->SetSelectedCharacter(2);
				}
			}
			else
			{
				ACombatAIController* SelectedController = Cast<ACombatAIController>(Orchestrator->GetPlayerCharacters()[MenuCharacterSelected]->GetController());
				if (SelectedController && SelectedController->SetNextCombatAction(ActionSlotPosition::RIGHT))
				{
					MenuCharacterSelected = -1;
					MenuWidget->SetSelectedCharacter(-1);
				}
			}
		}
	}
}

void ABasePlayerController::AttackLeft()
{
	if (IsInCombo)
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("AttackLeft()"))
		AttackKeyPressed(TEXT("AttackLeft"));
	}
	if (IsMenuOpen)
	{
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			if (MenuCharacterSelected == -1)
			{
				if (Orchestrator->GetPlayerCharacters().Num() >= 2)
				{
					MenuCharacterSelected = 1;
					MenuWidget->SetSelectedCharacter(1);
				}
			}
			else
			{
				ACombatAIController* SelectedController = Cast<ACombatAIController>(Orchestrator->GetPlayerCharacters()[MenuCharacterSelected]->GetController());
				if (SelectedController && SelectedController->SetNextCombatAction(ActionSlotPosition::LEFT))
				{
					MenuCharacterSelected = -1;
					MenuWidget->SetSelectedCharacter(-1);
				}
			}
		}
	}
}

void ABasePlayerController::AttackDown()
{
	if (IsInCombo)
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("AttackDown()"))
		AttackKeyPressed(TEXT("AttackDown"));
	}
	if (IsMenuOpen)
	{
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			if (MenuCharacterSelected == -1 && Orchestrator->GetPlayerCharacters().Num() >= 1)
			{
				MenuCharacterSelected = 0;
				MenuWidget->SetSelectedCharacter(0);
			}
			else
			{
				ACombatAIController* SelectedController = Cast<ACombatAIController>(Orchestrator->GetPlayerCharacters()[MenuCharacterSelected]->GetController());
				if (SelectedController && SelectedController->SetNextCombatAction(ActionSlotPosition::CENTER))
				{
					MenuCharacterSelected = -1;
					MenuWidget->SetSelectedCharacter(-1);
				}
			}
		}
	}
}

void ABasePlayerController::AttackUp()
{
	if (IsInCombo)
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("AttackUp()"))
		AttackKeyPressed(TEXT("AttackUp"));
	}
	if (IsMenuOpen)
	{
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator && MenuCharacterSelected != -1)
		{
			ACombatAIController* SelectedController = Cast<ACombatAIController>(Orchestrator->GetPlayerCharacters()[MenuCharacterSelected]->GetController());
			if (SelectedController && SelectedController->SetNextCombatAction(ActionSlotPosition::DEFAULT))
			{
				MenuCharacterSelected = -1;
				MenuWidget->SetSelectedCharacter(-1);
			}
		}
	}
}

void ABasePlayerController::OpenCombatMenu()
{
	if (IsInMenuInterval)
	{
		IsInMenuInterval = false;
		IsMenuOpen = true;
		if (MenuTimerWidget != nullptr)
		{
			MenuTimerWidget->RemoveFromViewport();
		}
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			Orchestrator->OpenCombatMenu();
			if (MenuWidget == nullptr)
			{
				MenuWidget = CreateWidget<UCombatMenuWidget>(this, MenuWidgetType);
			}
			MenuWidget->InitWidget(Orchestrator->GetPlayerCharacters());
			MenuWidget->AddToViewport();
		}
	}
	else if (IsMenuOpen)
	{
		IsMenuOpen = false;
		MenuCharacterSelected = -1;
		MenuWidget->SetSelectedCharacter(-1);
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			Orchestrator->CloseCombatMenu();
		}
		MenuWidget->RemoveFromViewport();
	}
}

void ABasePlayerController::Jump()
{
	if (GetCharacter() != nullptr)
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("Jump()"))
		GetCharacter()->Jump();
	}
}

void ABasePlayerController::AttackKeyPressed(FName AttackAction)
{
	if (Verbose) UE_LOG(LogTemp, Warning, TEXT("AttackKeyPressed()"))
	IsInCombo = false;
	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->RemoveFromViewport();
	}

	if (ComboAttackAction == AttackAction && (GetWorld()->GetRealTimeSeconds() - ComboStartTime) / ComboDuration > ComboPrecision)
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("Combo success"))
		ACombatOrchestrator* Orchestrator = Cast<ACombatOrchestrator>(GetPawn());
		if (Orchestrator)
		{
			Orchestrator->GetCurrentTurnController()->ComboSucceed();
		}
	}
	else
	{
		if (Verbose) UE_LOG(LogTemp, Warning, TEXT("Combo Failed"))
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

	if (IsInMenuInterval)
	{
		UpdateMenuWidget();
	}

	if (GetCharacter() == nullptr) return;

	if (MoveDirection.Size() > 0.1f)
	{
		AControllableCharacter* ControllableCharacter = Cast<AControllableCharacter>(GetCharacter());
		float CameraRotationZ = ControllableCharacter->GetCameraArm()->GetRelativeRotation().Euler().Z;

		if(Verbose) UE_LOG(LogTemp, Warning, TEXT("ABasePlayerController::Tick CameraRotationZ %f"), CameraRotationZ)

		SetControlRotation(MoveDirection.RotateAngleAxis(CameraRotationZ,FVector::UpVector).Rotation());
		GetCharacter()->AddMovementInput(GetCharacter()->GetActorForwardVector() * MoveDirection.Size());
	}
	else
	{
		SetControlRotation(GetCharacter()->GetActorRotation());
	}
}

void ABasePlayerController::NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType, FName AttackAction)
{
	if (Verbose) UE_LOG(LogTemp, Warning, TEXT("NotifyComboStart()"))
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
	if (CurrentAttackWidget != nullptr)
	{
		CurrentAttackWidget->UpdateElapseTime(GetWorld()->GetRealTimeSeconds() - ComboStartTime);
	}
}

void ABasePlayerController::NotifyComboEnd()
{
	if (Verbose) UE_LOG(LogTemp, Warning, TEXT("NotifyComboEnd()"))
	if (IsInCombo)
	{
		IsInCombo = false;
		if (CurrentAttackWidget != nullptr)
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

void ABasePlayerController::NotifyMenuStart(float MenuDuration)
{
	IsInMenuInterval = true;
	MenuStartTime = GetWorld()->GetRealTimeSeconds();
	if (MenuTimerWidget == nullptr)
	{
		MenuTimerWidget = CreateWidget<UCombatWidget>(this, MenuTimerWidgetType);
	}
	MenuTimerWidget->ResetWidget(MenuDuration);
	MenuTimerWidget->AddToViewport();
}

void ABasePlayerController::NotifyMenuEnd()
{
	if (IsInMenuInterval)
	{
		IsInMenuInterval = false;
		if (MenuTimerWidget != nullptr)
		{
			MenuTimerWidget->RemoveFromViewport();
		}
	}
}

void ABasePlayerController::UpdateMenuWidget()
{
	if (MenuTimerWidget != nullptr)
	{
		MenuTimerWidget->UpdateElapseTime(GetWorld()->GetRealTimeSeconds() - MenuStartTime);
	}
}
