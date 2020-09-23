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
#include "BasePlayerController.h"

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
	if (PlayerCharacters[0] == nullptr || EnemyCharacters[0] == nullptr) return;

	//TODO: setup combat camera
	//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.f);

	TurnQueue.Dequeue(CurrentTurnController);
	TurnQueue.Enqueue(CurrentTurnController);
	StartMenuInterval();
}

void ACombatOrchestrator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatOrchestrator::Initialize(AControllableCharacter* APlayerCharacter, ABaseCharacter* EnemyCharacter)
{
	if (APlayerCharacter == nullptr || EnemyCharacter == nullptr) return;
	PlayerCharacter = APlayerCharacter;

	CombatCenter = FMath::Lerp(PlayerCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation(), .5f);
	CombatCamera->SetWorldLocation(CombatCenter + FVector(0.f, 0.f, 500.f));
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CombatCamera->GetComponentLocation(), CombatCenter);
	CombatCamera->SetWorldRotation(CameraRotation);

	//DrawDebugSphere(GetWorld(), CombatCenter, CombatRadius, 16, FColor::Yellow, true);

	PlayerCharacter->StartCombat(this);

	PlayerCharacters.Add(Cast<ABaseCharacter>(PlayerCharacter));
	for (ABaseCharacter* Ally : PlayerCharacter->GetAllies())
	{
		PlayerCharacters.Add(Ally);
	}
	EnemyCharacters.Add(Cast<ABaseCharacter>(EnemyCharacter));
	for (ABaseCharacter* Ally : EnemyCharacter->GetAllies())
	{
		EnemyCharacters.Add(Ally);
	}

	//TODO: find order of combat
	PlayerDirection = CalculateCombatDirection(PlayerCharacter->GetActorLocation());
	for (int x = 0; x < PlayerCharacters.Num(); ++x)
	{
		ACombatAIController* PlayerController = Cast<ACombatAIController>(PlayerCharacters[x]->GetController());
		PlayerController->StartCombat(EnemyCharacter, FindCombatPosition(PlayerDirection, x));
		PlayerController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
		TurnQueue.Enqueue(PlayerController);
	}
	EnemyDirection = CalculateCombatDirection(EnemyCharacter->GetActorLocation());
	for (int x = 0; x < EnemyCharacters.Num(); ++x)
	{
		ACombatAIController* EnemyController = Cast<ACombatAIController>(EnemyCharacters[x]->GetController());
		EnemyController->StartCombat(PlayerCharacter, FindCombatPosition(-EnemyDirection, x));
		EnemyController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
		TurnQueue.Enqueue(EnemyController);
	}
}

void ACombatOrchestrator::AddCharacter(ABaseCharacter* NewCharacter, bool bOnPlayerSide)
{
	ACombatAIController* NewController = Cast<ACombatAIController>(NewCharacter->GetController());
	if (bOnPlayerSide)
	{
		//TODO: focus on non dead character
		NewController->StartCombat(EnemyCharacters[0], FindCombatPosition(PlayerDirection, PlayerCharacters.Num()));
		PlayerCharacters.Add(Cast<ABaseCharacter>(NewCharacter));
	}
	else
	{
		//TODO: focus on non dead character
		NewController->StartCombat(PlayerCharacters[0], FindCombatPosition(-EnemyDirection, EnemyCharacters.Num()));
		EnemyCharacters.Add(Cast<ABaseCharacter>(NewCharacter));
	}
	NewController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
	TurnQueue.Enqueue(NewController);

	for (ABaseCharacter* Ally : NewCharacter->GetAllies())
	{
		ACombatAIController* AllyController = Cast<ACombatAIController>(Ally->GetController());

		if (bOnPlayerSide)
		{
			AllyController->StartCombat(EnemyCharacters[0], FindCombatPosition(PlayerDirection, PlayerCharacters.Num()));
			PlayerCharacters.Add(Ally);
		}
		else
		{
			AllyController->StartCombat(PlayerCharacters[0], FindCombatPosition(-EnemyDirection, EnemyCharacters.Num()));
			EnemyCharacters.Add(Ally);
		}
		AllyController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
		TurnQueue.Enqueue(AllyController);
	}
}

ACombatAIController* ACombatOrchestrator::GetCurrentTurnController()
{
	return CurrentTurnController;
}

void ACombatOrchestrator::EndCurrentTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Orchestrator::EndCurrentTurn"))
	if (!HasOneCharacterAlive(PlayerCharacters))
	{
		EndCombat(false);
	}
	else if (!HasOneCharacterAlive(EnemyCharacters))
	{
		EndCombat(true);
	}
	else
	{
		TurnQueue.Dequeue(CurrentTurnController);
		TurnQueue.Enqueue(CurrentTurnController);
	}

	if (CurrentTurnController->GetCharacter() == PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Orchestrator::EndCurrentTurn - Player Turn"))
		StartMenuInterval();
	}
	else
	{
		StartNextTurn();
	}
}

void ACombatOrchestrator::StartMenuInterval()
{
	GetWorld()->GetTimerManager().SetTimer(MenuTimeHandle, this, &ACombatOrchestrator::StartNextTurn, MenuTimeWindow);

	ABasePlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	if (PlayerController == nullptr) return;
	PlayerController->NotifyMenuStart(MenuTimeWindow);
}

void ACombatOrchestrator::OpenCombatMenu()
{
	GetWorld()->GetTimerManager().ClearTimer(MenuTimeHandle);
}

void ACombatOrchestrator::CloseCombatMenu()
{
	StartNextTurn();
}

void ACombatOrchestrator::StartNextTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Orchestrator::StartNextTurn"))

	ABasePlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	if (PlayerController == nullptr) return;
	PlayerController->NotifyMenuEnd();

	CurrentTurnController->StartTurn(PlayerCharacters, EnemyCharacters);
}

bool ACombatOrchestrator::HasOneCharacterAlive(TArray<ABaseCharacter*> Characters)
{
	for (ABaseCharacter* Character : Characters)
	{
		if (Character->IsAlive())
		{
			return true;
		}
	}
	return false;
}

FVector ACombatOrchestrator::FindCombatPosition(FVector InitialDirection, int CharacterIndex)
{
	int Offset = CharacterIndex / 2;
	if (CharacterIndex % 2 != 0)
	{
		Offset = -1 * (Offset + 1);
	}
	FVector RotatedPosition = InitialDirection.RotateAngleAxis(Offset * CombatPositionDegreeOffset, FVector::UpVector);
	//DrawDebugPoint(GetWorld(), CombatCenter + RotatedPosition, 8.f, FColor::Yellow, true);
	return CombatCenter + RotatedPosition;
}

FVector ACombatOrchestrator::CalculateCombatDirection(FVector CharacterPosition)
{
	FVector CharacterDirection = (PlayerCharacter->GetActorLocation() - CombatCenter);
	CharacterDirection.Normalize();
	return CharacterDirection * CombatRadius;
}

void ACombatOrchestrator::EndCombat(bool PlayerWon)
{
	if (PlayerWon)
	{
		for (ABaseCharacter* Character : PlayerCharacters)
		{
			ACombatAIController* PlayerController = Cast<ACombatAIController>(Character->GetController());
			PlayerController->StopCombat();
		}
		PlayerCharacter->StopCombat();
		GetWorld()->DestroyActor(this);
	}
	else
	{
		//TODO reset enemy ai to wander
	}
}

TArray<ABaseCharacter*> ACombatOrchestrator::GetPlayerCharacters()
{
	return PlayerCharacters;
}