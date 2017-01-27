// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "TurretPylon.h"
#include "TurretPlaceholderBase.h"


// Sets default values
ATurretPlaceholderBase::ATurretPlaceholderBase()
{
	PrimaryActorTick.bCanEverTick = true;
	sceneComp = CreateDefaultSubobject<USceneComponent>("BaseSceneComponent");
	SetRootComponent(sceneComp);

	turretPh = CreateDefaultSubobject<USkeletalMeshComponent>("TurretMesh");
	turretPh->AttachToComponent(sceneComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	colComp = CreateDefaultSubobject<UBoxComponent>("colShape");
	colComp->AttachToComponent(sceneComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

// Called when the game starts or when spawned
void ATurretPlaceholderBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATurretPlaceholderBase::OnConstruction(const FTransform& Transform)
{
	turretDynMat = UMaterialInstanceDynamic::Create(turretInstMat, this);
	turretPh->SetMaterial(0, turretDynMat);
	ableToSpawn = true;
	turretDynMat->SetVectorParameterValue("Color", clear);
	Super::OnConstruction(Transform);
}

// Called every frame
void ATurretPlaceholderBase::Tick(float DeltaTime)
{
	TArray<AActor*> overlappers;
	colComp->GetOverlappingActors(overlappers);

	if (overlappers.Num() >= 1)
	{
		bool valid = true;
		for (auto actor : overlappers)
		{
			if (!actor->IsA(ATurretPylon::StaticClass()))
			{
				valid = false;
				break;
			}
		}
		if (valid)
		{
			if (!ableToSpawn)
			{
				ableToSpawn = true;
				turretDynMat->SetVectorParameterValue("Color", clear);
			}
		}
		else
		{
			if (ableToSpawn)
			{
				ableToSpawn = false;
				turretDynMat->SetVectorParameterValue("Color", somethingInTheWay);
			}
		}
	}
	else
	{
		if (ableToSpawn)
		{
			ableToSpawn = false;
			turretDynMat->SetVectorParameterValue("Color", somethingInTheWay);
		}
	}
	Super::Tick(DeltaTime);
}

void ATurretPlaceholderBase::SetVisibility(bool visible)
{
	turretPh->SetVisibility(visible, true);
	if (!visible)
	{
		colComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		colComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
