// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "LaserOrbCpp.h"
#include "HoverTankController.h"
#include "EnemyEventHandler.h"
#include "DonAINavigation/Classes/DonNavigationManager.h"
#include "LaserSentryController.generated.h"

USTRUCT()
struct FShortestRouteResult {
	GENERATED_BODY()
	FVector turretLoc;
	float distance;
	int count;
	int maxCount;
	bool searchInProgress = false;
};

/**
*
*/
UCLASS()
class TANKTRYCPP_API ALaserSentryController : public AAIController
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FFindingTargetReturnHandler, ABaseTurret*, newTargetActor, bool, successful);

	ALaserSentryController(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* inPawn) override;

	UFUNCTION(BlueprintCallable, Category = Ai)
		void FindNearestTarget(FFindingTargetReturnHandler inRetHandler);

	UFUNCTION()
		void CtrlPawnIsHurt(float amountOfDmg);
	UFUNCTION()
		void PathFindingResultReturned(const FDoNNavigationQueryData& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		AActor *AiTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
		float fireRate = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		float optimalHeight = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		float optimalDistance = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		TArray<TEnumAsByte<EObjectTypeQuery>> pathBlockingObjects;

	UEnemyEventHandler* EEHandler;
	FFindingTargetReturnHandler retHandler;

	//	ABaseTurret* GetClosestTurret(FD)
private:
	FRotator interpFaceDirection;
	FRotator trueFaceDirection;

	FTimerHandle shootingTimer;
	void ShootingTimeElapsed();

	ALaserOrbCpp* ControlledPawn;

	ADonNavigationManager* navManager;
	FDoNNavigationResultHandler resultHandler;
	FDonNavigationDynamicCollisionDelegate dynamicColHandler;
	FShortestRouteResult lowestTargetActor;

	TArray<ABaseTurret*>* allTurretsPointer;
	bool searchConduncting = false;
	//float giantForLoopInd;
};
