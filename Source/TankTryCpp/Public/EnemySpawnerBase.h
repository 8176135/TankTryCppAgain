// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TankStateCpp.h"
#include "EnemySpawnPoints.h"
#include "CppFunctionList.h"
#include "EnemySpawnerBase.generated.h"

UCLASS()
class TANKTRYCPP_API AEnemySpawnerBase : public AActor
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		//TSubclassOf<AActor> spawnableActor;
	UPROPERTY(BlueprintReadOnly, Category = Basics)
		FTimerHandle spawnTimerHdl;
	UPROPERTY(BlueprintReadOnly, Category = Basics)
		ATankStateCpp* cppTankState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		float spawnRateMultiplier = 3;

private:
	FVector SpawnLoc;
	TArray<AEnemySpawnPoints*> listOfSpawnPoints;

private:
	static float SpawnRateGraph(float level, float multiplier);
	void ApplyEfx();
	UFUNCTION()
	void TimeToSpawn(FVector currentSpawnLocation, TSubclassOf<APawn> spawnableActor);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Effects)
		void SpawnEffects(FVector spawnLocation);
public:
	// Sets default values for this actor's properties
	AEnemySpawnerBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;



};
