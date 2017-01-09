// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "SpecCamera.h"


// Sets default values
ASpecCamera::ASpecCamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootSceComp = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(rootSceComp);

	specCam = CreateDefaultSubobject<UCameraComponent>("SpectatorCamera");
	specCam->AttachToComponent(rootSceComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	EEHandler = CreateDefaultSubobject<UEnemyEventHandler>("EEHandlerForAllies");
	transTimeline = CreateDefaultSubobject<UTimelineComponent>("TransitionTimeline");
	InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
}

// Called when the game starts or when spawned
void ASpecCamera::BeginPlay()
{
	transTimeline->AddInterpFloat(transCurve, InterpFunction, "Floaty");
	Super::BeginPlay();
}

// Called every frame
void ASpecCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpecCamera::StartTransition(FVector target, FRotator targetRot)
{
	startingLoc = GetActorLocation();
	startingRot = GetActorRotation();
	endingLoc = target;
	endingRot = targetRot;
	transTimeline->PlayFromStart();
}

// Called to bind functionality to input
//void ASpecCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
//{
//	Super::SetupPlayerInputComponent(InputComponent);
//}

void ASpecCamera::TimelineFloatReturn(float val)
{
	if (val < 1)
	{
		SetActorLocationAndRotation(FMath::Lerp(startingLoc, endingLoc, val), FMath::Lerp(startingRot, endingRot, val));
	}
	else
	{
		transTimeline->Stop();
		SetActorLocationAndRotation(endingLoc, endingRot);
		EEHandler->TransFinDele.ExecuteIfBound();
	}

}
