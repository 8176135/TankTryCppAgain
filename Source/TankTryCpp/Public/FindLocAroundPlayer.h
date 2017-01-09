// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FindLocAroundPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TANKTRYCPP_API UFindLocAroundPlayer : public UBTTaskNode
{
	GENERATED_BODY()
public:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector NewLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float floatDistance = 100;
};
