// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "EnemySpawnPoints.h"


// Sets default values
AEnemySpawnPoints::AEnemySpawnPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootObj"));
	SceneComp->Mobility == EComponentMobility::Type::Static;

	MarkerComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("Marker"));
	MarkerComp->SetRelativeLocation(FVector(0, 0, 0));
	MarkerComp->SetHiddenInGame(false);

	WithinSpawnArea = false;
}

// Called when the game starts or when spawned
void AEnemySpawnPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnPoints::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

