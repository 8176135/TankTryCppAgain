// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LaserOrbCpp.h"
#include "EnemySpawnPoints.generated.h"

UCLASS()
class TANKTRYCPP_API AEnemySpawnPoints : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	TArray<TSubclassOf<APawn>> EnemyToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
		bool WithinSpawnArea;

	UPROPERTY(EditAnywhere)
		USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere)
		UBillboardComponent* MarkerComp;
public:	
	// Sets default values for this actor's properties
	AEnemySpawnPoints();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
