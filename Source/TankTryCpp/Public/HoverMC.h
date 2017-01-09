// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "HoverMC.generated.h"

/**
 *
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class TANKTRYCPP_API UHoverMC : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void UpdateComponentVelocity() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float maxSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float turningTime = 0.5f;

	float speedModifier = 1;
private:
	//FVector oldPosition = FVector(0, 0, 0);
	//FVector trueMovThisFrame = FVector(0,0,0);
	FVector oldVelocity = FVector(0, 0, 0);
	bool stopping = false;
};
