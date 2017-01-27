// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HoverTank.h"
#include "DonAINavigation/Classes/DonNavigationManager.h"
#include "VirusCalc.generated.h"

USTRUCT()
struct FVirusPart
{
	GENERATED_USTRUCT_BODY()

		FVector Location;

	bool dead = false;
	bool blocked = false;
	float numOfFullSizedNeighbours;
	//UStaticMeshComponent* meshComponent;
	int virusIndex;
//	UPostProcessComponent* PPComponent;
	//UBoxComponent* PPVolume;
	USphereComponent* EnemyInfluenceVolume;

	bool fading = false;
	bool full = false;

	float lifetime;
	float fadingTime;

	float influenced;
	//FORCEINLINE bool IsDead() const	{ return _dead; }
private:


public:
	FVirusPart() {}
	FVirusPart(FVector realLoc, bool isBlocked, int inVirusIndex, /*UPostProcessComponent* PPComp, UBoxComponent* boxComp,*/ USphereComponent* influenceSphere, float startingTimeToLive, float inFadeTime)
	{
		Location = realLoc;
		EnemyInfluenceVolume = influenceSphere;
		virusIndex = inVirusIndex;
		blocked = isBlocked;
		lifetime = startingTimeToLive;
		fadingTime = inFadeTime;
		//PPComponent = PPComp;
		//PPVolume = boxComp;
	}

	void UpdateVirusIndex(int removedIndex, int delta = -1)
	{
		if (virusIndex > removedIndex)	
			virusIndex += delta;
	}
};

USTRUCT()
struct FppXValueStorage
{
	GENERATED_BODY()
public:
	float WhiteTint = 0;
	float LensFlareIntensity = 0;
	float BloomIntensity = 0;
	inline void UpdateValues(float deltaTime)
	{
		WhiteTint += deltaTime;
		LensFlareIntensity += deltaTime;
		BloomIntensity += deltaTime;
	}
};

USTRUCT()
struct FOverlapStorage
{
	GENERATED_BODY()
public:
	AActor* actor;
	int count = 0;

	FOverlapStorage() {}
	FOverlapStorage(AActor* inActor)
	{
		count = 1;
		actor = inActor;
	}
};

UCLASS()
class TANKTRYCPP_API AVirusCalc : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVirusCalc();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//void ForceUpdate();
	void SpawningVirus(FVector attackLoc);
	void QueueVirusesToSpawn(FVector attackerLoc);
	void KillVirus(FVector deadLoc);
	void CheckForNeighbours(FVector BlockToCheck, bool invert);

	//	UFUNCTION()
	//		void OverlapBegins(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	//			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//	UFUNCTION()
	//		void OverlapEnds(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		//FVirusWholeXYZ allVirusData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BoxSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timerLength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawnSpeed = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float totalLifetime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fadingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dmgPerSecond = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float scrapPerBlockPerSecond = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float enemyInfluenceRadius = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleQueryChannels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> virusDmgTp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* fadingMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* sparksPS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* ppShapes;
	USceneComponent* sceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* boxMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* billboardMarker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPostProcessSettings deadPPSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInstancedStaticMeshComponent* instancedVirus;

	TMap<FVector, FVirusPart> allVirusData;

	UFUNCTION()
		void ClearVirusNavBlock(FVector blockToClear);

protected:
	ATankStateCpp* trueTankState;
private:
	FVector currentLocation = FVector(0, 0, 0);
	TArray<FVector> attackLocations;
	FTimerHandle spawningTimer;
	const FVector allDirections[6] = { FVector(1, 0, 0) , FVector(-1, 0, 0) ,FVector(0, 1, 0) ,FVector(0, -1, 0),FVector(0, 0, 1),FVector(0, 0, -1) };
	//VirusSpawningParms
//	FCollisionObjectQueryParams vOQP;
	FCollisionShape vColShape;
	AHoverTank* player;

	FppXValueStorage storage;
	FPostProcessSettings unifiedPPSettings;
	int numOfCompBeingOverlapped = 0;

	ADonNavigationManager* navManager;
private:
	void SpawningTimeElapsed();
	void GetWorldLocation(FVector boxPos);
};

