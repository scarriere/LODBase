// Copyright Samuel Carriere 2020


#include "MovingActor.h"
#include "Kismet/KismetMathLibrary.h"

AMovingActor::AMovingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TargetTransformation = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
	TargetTransformation->SetupAttachment(RootComponent);

	MovingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Moving Mesh"));
	MovingMesh->SetupAttachment(RootComponent);
}

void AMovingActor::BeginPlay()
{
	Super::BeginPlay();

	if (MovingActor == nullptr)
	{
		InitialTransform = MovingMesh->GetRelativeTransform();
	}
	else
	{
		InitialTransform = MovingActor->GetActorTransform();
	}
}

void AMovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Moving)
	{
		MovingTime = FMath::Clamp(MovingTime + DeltaTime, 0.f, Time);

		if (MovingActor == nullptr)
		{
			FTransform NewTransform = UKismetMathLibrary::TLerp(InitialTransform, TargetTransformation->GetRelativeTransform(), MovingTime / Time);
			MovingMesh->SetRelativeTransform(NewTransform);
		}
		else
		{

			FTransform NewTransform = UKismetMathLibrary::TLerp(InitialTransform, UKismetMathLibrary::ComposeTransforms(TargetTransformation->GetRelativeTransform(), this->GetActorTransform()), MovingTime / Time);
			MovingActor->SetActorTransform(NewTransform);
		}

		if (MovingTime >= Time)
		{
			Moving = false;
		}
	}
}

void AMovingActor::StartMovement()
{
	Moving = true;
}
