// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "TurretPylon.h"


// Sets default values
ATurretPylon::ATurretPylon()
{

	FAttachmentTransformRules tempRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	PrimaryActorTick.bCanEverTick = true;
	baseScene = CreateDefaultSubobject<USceneComponent>("baseScene");
	SetRootComponent(baseScene);
	PortalMinimiumRange = CreateDefaultSubobject<UCapsuleComponent>("MinRange");
	PortalMinimiumRange->AttachToComponent(baseScene, tempRules);
	PortalSystem = CreateDefaultSubobject<UParticleSystemComponent>("Portal");
	PortalSystem->AttachToComponent(baseScene, tempRules);

	BuildRadiusCol = CreateDefaultSubobject<USphereComponent>("BuildRadius");
	BuildRadiusCol->AttachToComponent(baseScene, tempRules);
	EnemyInfluenceRadius = CreateDefaultSubobject<USphereComponent>("EnemyInflu");
	EnemyInfluenceRadius->AttachToComponent(baseScene, tempRules);

	ShowBuildable = CreateDefaultSubobject<UDecalComponent>("buildableDecal");
	ShowBuildable->AttachToComponent(baseScene, tempRules);
}

// Called when the game starts or when spawned
void ATurretPylon::BeginPlay()
{
	BuildRadiusCol->SetSphereRadius(0);
	Super::BeginPlay();

}

// Called every frame
void ATurretPylon::Tick(float DeltaTime)
{
	FHitResult hitRes;
	GetWorld()->LineTraceSingleByObjectType(hitRes, BuildRadiusCol->GetComponentLocation(), BuildRadiusCol->GetComponentLocation() + FVector(0, 0, BuildRadiusCol->GetUnscaledSphereRadius()), ObjectsToCareAbout, FCollisionQueryParams());
	if (hitRes.IsValidBlockingHit())
	{
		float halfSize = FMath::Pow(FMath::Square(BuildRadiusCol->GetUnscaledSphereRadius()) - FMath::Square(hitRes.Distance), 0.5);
		UCppFunctionList::PrintString("%f", halfSize);
		//ShowBuildable->SetRelativeScale3D(FVector())
	}
	float TotalEnemyInfluence = 0;

	if (BuildRadiusCol->GetUnscaledSphereRadius() < maxBuildRadius)
	{
		BuildRadiusCol->SetSphereRadius(BuildRadiusCol->GetUnscaledSphereRadius() + DeltaTime * (growSpeed * (1 - TotalEnemyInfluence)));
		if (BuildRadiusCol->GetUnscaledSphereRadius() > maxBuildRadius)
		{
			BuildRadiusCol->SetSphereRadius(maxBuildRadius);
		}
	}
	Super::Tick(DeltaTime);

}

