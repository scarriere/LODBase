// Copyright Samuel Carriere 2020


#include "CombatOrchestrator.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "BaseCharacter.h"
#include "CombatAIController.h"
#include "ControllableCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACombatOrchestrator::ACombatOrchestrator()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CombatCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Combat Camera"));
	RootComponent->SetupAttachment(RootComponent);
}

void ACombatOrchestrator::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerController == nullptr || EnemyController == nullptr) return;

	PlayerController->StartCombat(CombatCenter);
	EnemyController->StartCombat(CombatCenter);

	//TODO: setup combat camera
	//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.f);

	// TODO: Wait for combat ready (might have animation)
	PlayerController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
	EnemyController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);

	//TODO: find order of combat
	PlayerController->StartTurn(EnemyController->GetCharacter());
}

void ACombatOrchestrator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatOrchestrator::Initialize(AControllableCharacter* PlayerCharacter, ABaseCharacter* EnemyCharacter)
{
	if (PlayerCharacter == nullptr || EnemyCharacter == nullptr) return;

	PlayerCharacter->StartCombat(this);

	PlayerController = Cast<ACombatAIController>(PlayerCharacter->GetController());
	EnemyController = Cast<ACombatAIController>(EnemyCharacter->GetController());

	CombatCenter = FMath::Lerp(PlayerCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation(), .5f);

	CombatCamera->SetWorldLocation(CombatCenter + FVector(0.f, 0.f, 500.f));
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CombatCamera->GetComponentLocation(), CombatCenter);
	CombatCamera->SetWorldRotation(CameraRotation);
}

bool ACombatOrchestrator::IsPlayerTurn()
{
	return bIsPlayerTurn;
}

void ACombatOrchestrator::ComboMiss()
{
	PlayerController->ComboFail();
}

void ACombatOrchestrator::ComboSucceed()
{
	//TODO apply damage to target
}

void ACombatOrchestrator::EndCurrentTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Turn End"))
	if (bIsPlayerTurn)
	{
		bIsPlayerTurn = false;
		EnemyController->StartTurn(PlayerController->GetCharacter());
	}
	else
	{
		//TODO: setup combat camera
		//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(PlayerCharacter, 1.f);

		//AControllableCharacter* PlayerCharacter = Cast<AControllableCharacter>(PlayerController->GetCharacter());
		//PlayerCharacter->StopCombat();

		//GetWorld()->DestroyActor(EnemyController->GetCharacter());
		//GetWorld()->DestroyActor(this);

		//TODO implement health system
		bIsPlayerTurn = true;
		PlayerController->StartTurn(EnemyController->GetCharacter());
	}
}
