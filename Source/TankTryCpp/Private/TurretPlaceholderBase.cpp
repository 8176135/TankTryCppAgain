// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "TurretPlaceholderBase.h"


// Sets default values
ATurretPlaceholderBase::ATurretPlaceholderBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurretPlaceholderBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretPlaceholderBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
