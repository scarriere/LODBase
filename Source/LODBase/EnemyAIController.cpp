// Copyright Samuel Carriere 2020


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetPawn()->GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(WanderTimeHandle, this, &AEnemyAIController::Wander, WanderInterval, true);
}

void AEnemyAIController::Wander()
{
	if (GetMoveStatus() != EPathFollowingStatus::Idle) return;
	if(Verbose) DrawDebugSphere(GetWorld(), InitialLocation, WanderRadius, 12, FColor::Green, false, 2.f);

	const UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(this);
	FNavLocation ResultLocation;
	if (navSystem->GetRandomReachablePointInRadius(InitialLocation, WanderRadius, ResultLocation))
	{
		if (Verbose) DrawDebugPoint(GetWorld(), ResultLocation.Location, 5.f, FColor::Red, false, 2.f);
		MoveToLocation(ResultLocation.Location);
	}
}

void AEnemyAIController::SeePlayer(AActor * Player)
{
	if (CombatStep != CombatStep::NOT_IN_COMBAT) return;

	if (Verbose) UE_LOG(LogTemp, Warning, TEXT("Player Became Visible"))
	GetWorldTimerManager().ClearTimer(WanderTimeHandle);
	MoveToActor(Player);
}