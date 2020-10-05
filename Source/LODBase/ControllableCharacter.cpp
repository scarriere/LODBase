// Copyright Samuel Carriere 2020


#include "ControllableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "CombatAIController.h"
#include "BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CombatOrchestrator.h"
#include "Components/WidgetComponent.h"

AControllableCharacter::AControllableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetVisibility(false);
}

void AControllableCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultController = Cast<ABasePlayerController>(GetController());
	CombatAIController = GetWorld()->SpawnActor<ACombatAIController>(CombatAIControllerType);
}

void AControllableCharacter::StartCombat(ACombatOrchestrator* Orchestractor)
{
	DefaultController->UnPossess();
	DefaultController->Possess(Orchestractor);
	DefaultController->SetViewTarget(Orchestractor);
	DefaultController->SetControllerState(ControllerState::COMBAT);
	CombatAIController->Possess(this);
}

void AControllableCharacter::StopCombat()
{
	CombatAIController->UnPossess();
	DefaultController->Possess(this);
	DefaultController->SetViewTarget(this);
	DefaultController->SetControllerState(ControllerState::FREE_WALK);
}

void AControllableCharacter::EncounterEnemy(ABaseCharacter * Enemy)
{
	ACombatOrchestrator* CombatOrchestrator = Cast<ACombatOrchestrator>(UGameplayStatics::GetActorOfClass(GetWorld(), CombatOrchestratorType));
	if (CombatOrchestrator == nullptr)
	{
		CombatOrchestrator = GetWorld()->SpawnActorDeferred<ACombatOrchestrator>(CombatOrchestratorType, GetActorTransform());
		StartCombat(CombatOrchestrator);
		CombatOrchestrator->Initialize(this, Enemy);
		CombatOrchestrator->FinishSpawning(GetActorTransform());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy interfere with combat"))
		CombatOrchestrator->AddCharacter(Enemy);
	}
}

ABasePlayerController* AControllableCharacter::GetDefaultController()
{
	return DefaultController;
}

USpringArmComponent* AControllableCharacter::GetCameraArm()
{
	return CameraArm;
}

void AControllableCharacter::StartInteraction()
{
	InteractionWidgetComponent->SetVisibility(true);
}

void AControllableCharacter::StopInteraction()
{
	InteractionWidgetComponent->SetVisibility(false);
}