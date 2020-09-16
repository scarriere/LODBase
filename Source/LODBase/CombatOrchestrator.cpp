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
	for (ABaseCharacter* Character : PlayerCharacters)
	{
		ACombatAIController* PlayerController = Cast<ACombatAIController>(Character->GetController());
		PlayerController->StartCombat(EnemyCharacter);
		PlayerController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
		TurnQueue.Enqueue(PlayerController);
	}
	for (ABaseCharacter* Character : EnemyCharacters)
	{
		ACombatAIController* EnemyController = Cast<ACombatAIController>(Character->GetController());
		EnemyController->StartCombat(PlayerCharacter);
		EnemyController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
		TurnQueue.Enqueue(EnemyController);
	}
}

void ACombatOrchestrator::AddCharacter(ABaseCharacter* NewCharacter, bool bOnPlayerSide)
{
	ACombatAIController* NewController = Cast<ACombatAIController>(NewCharacter->GetController());
	//TODO: focus on non dead character
	NewController->StartCombat(PlayerCharacters[0]);
	NewController->EndTurnFunc.BindUObject(this, &ACombatOrchestrator::EndCurrentTurn);
	TurnQueue.Enqueue(NewController);

	if (bOnPlayerSide)
	{
		PlayerCharacters.Add(Cast<ABaseCharacter>(NewCharacter));
	}
	else
	{
		EnemyCharacters.Add(Cast<ABaseCharacter>(NewCharacter));
	}
}

ACombatAIController* ACombatOrchestrator::GetCurrentTurnController()
{
	return CurrentTurnController;
}

void ACombatOrchestrator::EndCurrentTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Turn End"))
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
		UE_LOG(LogTemp, Warning, TEXT("Player Turn"))
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
	UE_LOG(LogTemp, Warning, TEXT("Turn Start"))

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