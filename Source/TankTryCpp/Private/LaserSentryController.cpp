// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "LaserSentryController.h"
#include "CppFunctionList.h"
#include "DrawDebugHelpers.h"
#include "LaserSentryCustomPFC.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


ALaserSentryController::ALaserSentryController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULaserSentryCustomPFC>(TEXT("PathFollowingComponent")))
{
	SetActorTickEnabled(true);
	resultHandler.BindUFunction(this, "PathFindingResultReturned");

}

void ALaserSentryController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(shootingTimer, this, &ALaserSentryController::ShootingTimeElapsed, 60.0f / fireRate, true);
	allTurretsPointer = &Cast<AHoverTankController>(GetWorld()->GetFirstPlayerController())->allTurretsSpawned;
	for (TActorIterator<ADonNavigationManager> It(GetWorld(), ADonNavigationManager::StaticClass()); It; ++It)
	{
		navManager = *It;
		break;
	}
}

void ALaserSentryController::Tick(float deltaTime)
{
	if (IsValid(AiTarget) && IsValid(ControlledPawn) && Blackboard->GetValueAsEnum("State") != 2)
	{
		FVector deltaVector = (AiTarget->GetActorLocation() - ControlledPawn->GetActorLocation());
		trueFaceDirection = deltaVector.GetSafeNormal().Rotation();
		interpFaceDirection = FMath::RInterpTo(ControlledPawn->GetActorRotation(), trueFaceDirection, deltaTime, 4);
		ControlledPawn->SetActorRotation(interpFaceDirection);
		if (Blackboard->GetValueAsEnum("State") == 1)
		{
			ControlledPawn->AddMovementInput(FVector(0, 0, (optimalHeight - ControlledPawn->GetActorLocation().Z) / 300));
			ControlledPawn->AddMovementInput(deltaVector.GetSafeNormal() * (((deltaVector.Size() - optimalDistance)) / 200));
		}
	}
	Super::Tick(deltaTime);
}

void ALaserSentryController::SetPawn(APawn* inPawn)
{
	if (IsValid(inPawn))
	{
		ControlledPawn = Cast<ALaserOrbCpp>(inPawn);
		EEHandler = ControlledPawn->EEHandler;
		if ((*EEHandler).HurtDele.IsBound())
		{
			(*EEHandler).HurtDele.Unbind();
		}
		(*EEHandler).HurtDele.BindUFunction(this, FName("CtrlPawnIsHurt"));
	}
	Super::SetPawn(inPawn);
}

void ALaserSentryController::FindNearestTarget(FFindingTargetReturnHandler inRetHandler)
{
	if (lowestTargetActor.searchInProgress)
	{
		UCppFunctionList::PrintString("Pathfinding already in progress");
		return;
	}
	if (allTurretsPointer->Num() == 0)
	{
		UCppFunctionList::PrintString("No More Targets");
		return;
	}
	lowestTargetActor.distance = -1;
	lowestTargetActor.turretLoc = FVector();
	lowestTargetActor.count = 0;
	lowestTargetActor.maxCount = allTurretsPointer->Num();
	FDoNNavigationQueryParams query = FDoNNavigationQueryParams();
	lowestTargetActor.count++;
	lowestTargetActor.searchInProgress = true;
	navManager->SchedulePathfindingTask(ControlledPawn, (*allTurretsPointer)[0]->mainTurretSke->Bounds.GetBox().GetCenter(), query,
		FDoNNavigationDebugParams(false, false, false, false, 2), resultHandler, dynamicColHandler);


	retHandler = inRetHandler;
}

void ALaserSentryController::CtrlPawnIsHurt(float amountOfDmg)
{
	if (Blackboard->GetValueAsEnum("State") != 2 && FMath::FRand() < (amountOfDmg / 40))
	{
		Blackboard->SetValueAsEnum("State", 2);
	}
}

void ALaserSentryController::PathFindingResultReturned(const FDoNNavigationQueryData& Data)
{
	float distance;
	if (Data.PathSolutionOptimized.Num() == 0)
	{
		DrawDebugSphere(GetWorld(), Data.Destination, 40, 12, FColor::White, false, 5, 0, 2);
		UCppFunctionList::PrintString("No Optimized Path");
	}
	else
	{
		distance = (Data.Origin - Data.PathSolutionOptimized[0]).Size();

		for (int i = 1; i < Data.PathSolutionOptimized.Num(); ++i)
		{
			distance += (Data.PathSolutionOptimized[i] - Data.PathSolutionOptimized[i - 1]).Size();
		}
		if (lowestTargetActor.distance == -1)
		{
			lowestTargetActor.distance = distance;
			lowestTargetActor.turretLoc = Data.Destination;
		}
		else if (distance < lowestTargetActor.distance)
		{
			lowestTargetActor.distance = distance;
			lowestTargetActor.turretLoc = Data.Destination;
		}
	}
	if (lowestTargetActor.count < lowestTargetActor.maxCount)
	{
		FDoNNavigationQueryParams query = FDoNNavigationQueryParams();
		lowestTargetActor.count++;
		if (allTurretsPointer->Num() > lowestTargetActor.count - 1 && IsValid((*allTurretsPointer)[lowestTargetActor.count - 1]))
		{
			navManager->SchedulePathfindingTask(ControlledPawn, (*allTurretsPointer)[lowestTargetActor.count - 1]->mainTurretSke->Bounds.GetBox().GetCenter(), query,
				FDoNNavigationDebugParams(false, false, true, false, 2), resultHandler, dynamicColHandler);
		}
	}
	else
	{
		if (lowestTargetActor.distance == -1)
		{
			UCppFunctionList::PrintString("Got Nobody mate");
			lowestTargetActor.searchInProgress = false;
			retHandler.ExecuteIfBound(nullptr, false);
			return;
		}
		TArray<ABaseTurret*>* allTurrets = &Cast<AHoverTankController>(GetWorld()->GetFirstPlayerController())->allTurretsSpawned;
		FVector dest = lowestTargetActor.turretLoc;

		ABaseTurret* closestTurret = nullptr;/* = *allTurrets->FindByPredicate([dest](ABaseTurret* InItem) {
			if (InItem)
				return InItem->mainTurretSke->Bounds.GetBox().GetCenter().Equals(dest, 10);
			else
				return false;
		});*/
		for (ABaseTurret* turret : *allTurrets)
		{
			if (turret && turret->mainTurretSke->Bounds.GetBox().GetCenter().Equals(dest, 10))
			{
				closestTurret = turret;
			}
		}
		if (closestTurret)
		{
			UCppFunctionList::PrintString("Well something worked");
			lowestTargetActor.searchInProgress = false;
			retHandler.ExecuteIfBound(closestTurret, true);
		}
		lowestTargetActor.searchInProgress = false;
	}
}

void ALaserSentryController::ShootingTimeElapsed()
{
	if (Blackboard->GetValueAsEnum(FName("State")) == 1 && IsValid(AiTarget) && IsValid(ControlledPawn))
	{
		ControlledPawn->Shoot(AiTarget);
	}
}