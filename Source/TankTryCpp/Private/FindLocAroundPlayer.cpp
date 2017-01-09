// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "FindLocAroundPlayer.h"
#include "CppFunctionList.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UFindLocAroundPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	UBlackboardData *blackboard = GetBlackboardAsset();
	AAIController* controller = OwnerComp.GetAIOwner();
	if (controller)
	{

		UBlackboardComponent* Blackboard = controller->FindComponentByClass<UBlackboardComponent>();
		FVector playerLocation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation();
		FVector direction = (controller->GetPawn()->GetActorLocation() - playerLocation).GetSafeNormal();
		if (direction == FVector::ZeroVector)
		{
			return EBTNodeResult::Failed;
		}
		direction *= floatDistance;

		//Blackboard->SetValueAsVector(NewLocation.SelectedKeyName,UCppFunctionList::FindOrbitRadiusPos());
		UCppFunctionList::PrintVector(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation());

		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}


}
