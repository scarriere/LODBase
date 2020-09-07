// Copyright Samuel Carriere 2020


#include "CombatOrchestrator.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "BaseCharacter.h"
#include "CombatAIController.h"
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

	if (PlayerCharacter == nullptr || EnemyCharacter == nullptr) return;

	PlayerCharacter->StartCombat(CombatCenter);
	EnemyCharacter->StartCombat(CombatCenter);

	// Weird bug where the target get unset when unpossses is called
	GetWorld()->GetFirstPlayerController()->SetViewTarget(PlayerCharacter);
	//TODO: setup combat camera
	//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.f);

	// TODO: Wait for combat ready (might have animation)
	PlayerCharacter->GetCombatController()->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
	EnemyCharacter->GetCombatController()->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);

	//TODO: find order of combat
	//EnemyCharacter->GetCombatController()->StartTurn(PlayerCharacter);
	PlayerCharacter->GetCombatController()->StartTurn(EnemyCharacter);
}

void ACombatOrchestrator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatOrchestrator::Initialize(ABaseCharacter* APlayerCharacter, ABaseCharacter* AEnemyCharacter)
{
	if (APlayerCharacter == nullptr || AEnemyCharacter == nullptr) return;

	PlayerCharacter = APlayerCharacter;
	EnemyCharacter = AEnemyCharacter;

	CombatCenter = FMath::Lerp(PlayerCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation(), .5f);

	CombatCamera->SetWorldLocation(CombatCenter + FVector(0.f, 0.f, 500.f));
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CombatCamera->GetComponentLocation(), CombatCenter);
	CombatCamera->SetWorldRotation(CameraRotation);
}

void ACombatOrchestrator::EndCurrentTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Turn End"))
	if (IsPlayerTurn)
	{
		IsPlayerTurn = false;
		EnemyCharacter->GetCombatController()->StartTurn(PlayerCharacter);
	}
	else
	{
		//TODO: setup combat camera
		//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(PlayerCharacter, 1.f);

		PlayerCharacter->StopCombat();
		EnemyCharacter->StopCombat();

		GetWorld()->DestroyActor(EnemyCharacter);
		GetWorld()->DestroyActor(this);

		//IsPlayerTurn = true;
		//PlayerCharacter->GetCombatController()->StartTurn(EnemyCharacter);
	}
}