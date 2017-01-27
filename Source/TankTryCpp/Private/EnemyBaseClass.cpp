// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "EnemyBaseClass.h"


// Sets default values
AEnemyBaseClass::AEnemyBaseClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//// Called when the game starts or when spawned
//void AEnemyBaseClass::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void AEnemyBaseClass::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//
//}
//
//// Called to bind functionality to input
//void AEnemyBaseClass::SetupPlayerInputComponent(class UInputComponent* inputComponent)
//{
//	Super::SetupPlayerInputComponent(inputComponent);
//
//}

float AEnemyBaseClass::GetEnemyInfluence() const
{
	return virusInfluenceAmount;
}
