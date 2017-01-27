// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "EnemySpawnerBase.h"
#include "CppFunctionList.h"


// Sets default values
AEnemySpawnerBase::AEnemySpawnerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawnerBase::BeginPlay()
{
	cppTankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHdl, this, &AEnemySpawnerBase::ApplyEfx, SpawnRateGraph(cppTankState->EnemyLevel, spawnRateMultiplier), false);
	{
		TArray<AActor*> temp;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnPoints::StaticClass(), temp);
		listOfSpawnPoints.SetNum(temp.Num());
		for (int i = 0; i < temp.Num(); i++)
		{
			listOfSpawnPoints[i] = (Cast<AEnemySpawnPoints>(temp[i]));
		}
	}

	Super::BeginPlay();
}

// Called every frame
void AEnemySpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemySpawnerBase::SpawnRateGraph(float level, float multiplier)
{
	float answer = (multiplier / (0.1f * (level + 6)));
	return answer;
}

void AEnemySpawnerBase::ApplyEfx()
{
	FTimerDelegate EfxTimerDel;
	FTimerHandle temp = FTimerHandle();
	int randomIndex;
	int ittCount = 0;
	while (true)
	{
		if (listOfSpawnPoints.Num() == 0)
		{
			UCppFunctionList::PrintString("Game Over");
			return;
		}
		
		randomIndex = FMath::RandRange(0, listOfSpawnPoints.Num() - 1);

		ittCount++;
		if (listOfSpawnPoints[randomIndex]->spawningFromHere)
		{
			FHitResult hitRes;

			if (GetWorld()->LineTraceSingleByObjectType(hitRes, listOfSpawnPoints[randomIndex]->GetActorLocation(),
				listOfSpawnPoints[randomIndex]->GetActorLocation() + FVector(1, 0, 0), virusColType) &&
				(IsValid(hitRes.GetActor()) && hitRes.GetActor()->ActorHasTag("Virus")))
			{
				listOfSpawnPoints.RemoveAt(randomIndex);
			}
			else
			{
				break;
			}
		}
		if (ittCount > 150)
		{
			UCppFunctionList::PrintString("Why u fail me");
			GetWorld()->GetTimerManager().SetTimer(spawnTimerHdl, this, &AEnemySpawnerBase::ApplyEfx, SpawnRateGraph(cppTankState->EnemyLevel, spawnRateMultiplier), false);
			return;
		}
	}

	int spawnTypeIndex = FMath::RandRange(0, listOfSpawnPoints[randomIndex]->EnemyToSpawn.Num() - 1);
	SpawnLoc = listOfSpawnPoints[randomIndex]->GetActorLocation();
	EfxTimerDel.BindUFunction(this, FName("TimeToSpawn"), SpawnLoc, listOfSpawnPoints[randomIndex]->EnemyToSpawn[spawnTypeIndex]);
	GetWorld()->GetTimerManager().SetTimer(temp, EfxTimerDel, 0.25f, false);

	SpawnEffects(SpawnLoc);

}

void AEnemySpawnerBase::TimeToSpawn(FVector currentSpawnLocation, TSubclassOf<APawn> spawnableActor)
{
	FRotator spawnRot = FRotator(0, 0, 0);
	FActorSpawnParameters spawnParams;

	APawn* newPawn = GetWorld()->SpawnActor<APawn>(spawnableActor, SpawnLoc, spawnRot, spawnParams);
	if (IsValid(newPawn))
	{
		newPawn->SpawnDefaultController();
	}

	GetWorld()->GetTimerManager().SetTimer(spawnTimerHdl, this, &AEnemySpawnerBase::ApplyEfx, SpawnRateGraph(cppTankState->EnemyLevel, spawnRateMultiplier), false);
}
