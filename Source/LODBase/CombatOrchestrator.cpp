// Copyright Samuel Carriere 2020


#include "CombatOrchestrator.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "BaseCharacter.h"
#include "CombatAIController.h"

ACombatOrchestrator::ACombatOrchestrator()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ACombatOrchestrator::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCharacter == nullptr || EnemyCharacter == nullptr) return;

	PlayerCharacter->StartCombat(CombatCenter);
	EnemyCharacter->StartCombat(CombatCenter);

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
		PlayerCharacter->StopCombat();
		EnemyCharacter->StopCombat();

		GetWorld()->DestroyActor(this);
		//IsPlayerTurn = true;
		//PlayerController->StartTurn();
	}
}