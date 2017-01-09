// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"

#include "HoverMC.h"


void UHoverMC::BeginPlay()
{
	//oldPosition = UpdatedComponent->GetComponentLocation();
	Super::BeginPlay();
}

void UHoverMC::TickComponent(float deltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//UCppFunctionList::PrintString(TEXT("PMC"));
	if (ShouldSkipUpdate(deltaTime))
	{
		return;
	}

	Super::TickComponent(deltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	const AController* Controller = PawnOwner->GetController();
	if (IsValid(Controller) && Controller->IsLocalController())
	{
		//UCppFunctionList::PrintVector(GetInputVector());
		FVector desiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1) * deltaTime * maxSpeed;
		FVector oldPosition = UpdatedComponent->GetComponentLocation();
		FVector trueMovThisFrame = FMath::Lerp(oldVelocity, desiredMovementThisFrame, deltaTime / turningTime);
		oldVelocity = trueMovThisFrame;

		if (!trueMovThisFrame.IsNearlyZero())
		{
			FHitResult Hit;

			SafeMoveUpdatedComponent(trueMovThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, deltaTime, trueMovThisFrame);

				float percentOfHit = SlideAlongSurface(trueMovThisFrame, 1.0 - Hit.Time, Hit.Normal, Hit);

				FVector actualMovement = UpdatedComponent->GetComponentLocation() - oldPosition;
				oldVelocity = actualMovement;
			}
		}

		//else if (IsExceedingMaxSpeed(maxSpeed))
		//{
		//	Velocity = Velocity.GetSafeNormal() * maxSpeed;
		//}
		////FVector actualVelocity = UpdatedComponent->GetComponentLocation() - oldPosition;
		//trueVelocity = FMath::Lerp(oldVelocity, Velocity, deltaTime / turningTime);
		////UCppFunctionList::PrintVector(trueVelocity);
		//oldVelocity = trueVelocity;

		//FVector AiDesiredMovement = trueVelocity * deltaTime;

		//if (!AiDesiredMovement.IsNearlyZero())
		//{
		//	FHitResult Hit;
		//	SafeMoveUpdatedComponent(AiDesiredMovement, UpdatedComponent->GetComponentRotation(), true, Hit);

		//	if (Hit.IsValidBlockingHit())
		//	{
		//		HandleImpact(Hit, deltaTime, AiDesiredMovement);
		//		SlideAlongSurface(AiDesiredMovement, 1.0 - Hit.Time, Hit.Normal, Hit);
		//	}
		//}
		//UpdateComponentVelocity();
	}

	//oldPosition = UpdatedComponent->GetComponentLocation();
	//UCppFunctionList::PrintBool(PawnOwner->GetController()->IsLocalController());
}

void UHoverMC::UpdateComponentVelocity()
{
	if (UpdatedComponent)
	{
		UpdatedComponent->ComponentVelocity = Velocity;
	}
}
