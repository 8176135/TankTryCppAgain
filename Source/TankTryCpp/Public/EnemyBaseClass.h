// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "EnemyBaseClass.generated.h"

UCLASS()
class TANKTRYCPP_API AEnemyBaseClass : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBaseClass();

	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual float GetEnemyInfluence() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float virusInfluenceAmount;
	
};
