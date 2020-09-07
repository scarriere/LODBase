// Copyright Samuel Carriere 2020


#include "BaseAIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"

ABaseAIController::ABaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = GetPawn()->GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(WanderTimeHandle, this, &ABaseAIController::Wander, WanderInterval, true);
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseAIController::Wander()
{
	if (GetMoveStatus() != EPathFollowingStatus::Idle) return;
	DrawDebugSphere(GetWorld(), InitialLocation, WanderRadius, 12, FColor::Green, false, 2.f);
	
	const UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(this);
	FNavLocation ResultLocation;
	if (navSystem->GetRandomReachablePointInRadius(InitialLocation, WanderRadius, ResultLocation))
	{
		DrawDebugPoint(GetWorld(), ResultLocation.Location, 5.f, FColor::Red, false, 2.f);
		MoveToLocation(ResultLocation.Location);
	}
}

void ABaseAIController::SeePlayer(AActor * Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Became Visible"))
	GetWorldTimerManager().ClearTimer(WanderTimeHandle);
	MoveToActor(Player);
}