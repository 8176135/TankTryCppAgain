// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "EnemyEventHandler.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "SpecCamera.generated.h"

UCLASS()
class TANKTRYCPP_API ASpecCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpecCamera();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void StartTransition(FVector target, FRotator targetRot);

	UEnemyEventHandler* EEHandler;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* specCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* rootSceComp;
	UPROPERTY()
		UTimelineComponent* transTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* transCurve;

	FOnTimelineFloat InterpFunction{};

	UFUNCTION()
		void TimelineFloatReturn(float val);

	private:
		FVector startingLoc;
		FRotator startingRot;
		FVector endingLoc;
		FRotator endingRot;

};
