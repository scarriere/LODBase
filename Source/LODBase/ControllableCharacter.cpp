// Copyright Samuel Carriere 2020


#include "ControllableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "CombatAIController.h"
#include "BasePlayerController.h"
#include "CombatWidget.h"

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
	CombatAIController = GetWorld()->SpawnActor<ACombatAIController>(CombatAIControllerType);
}

void AControllableCharacter::Tick(float DeltaTime)
{
	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->UpdateElapseTime(GetWorld()->GetRealTimeSeconds() - ComboStartTime);
	}
}

void AControllableCharacter::StartCombat(APawn* Orchestractor)
{
	DefaultController->UnPossess();
	DefaultController->Possess(Orchestractor);
	DefaultController->SetViewTarget(this);
	CombatAIController->Possess(this);
}

void AControllableCharacter::StopCombat()
{
	CombatAIController->UnPossess();
	DefaultController->Possess(this);
}

void AControllableCharacter::NotifyComboStart(float TotalDuration, TSubclassOf<UCombatWidget> WidgetType)
{
	CombatAIController->NotifyComboBegin(TotalDuration);
	ComboStartTime = GetWorld()->GetRealTimeSeconds();

	if (CurrentAttackWidget == nullptr)
	{
		CurrentAttackWidget = CreateWidget<UCombatWidget>(DefaultController, WidgetType);
	}

	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->ResetWidget(TotalDuration);
		CurrentAttackWidget->AddToViewport();
	}
}

void AControllableCharacter::NotifyComboEnd()
{
	CombatAIController->NotifyComboEnd();

	if (CurrentAttackWidget)
	{
		CurrentAttackWidget->RemoveFromViewport();
	}
}