// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TurretPylon.generated.h"

UCLASS()
class TANKTRYCPP_API ATurretPylon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurretPylon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = Components)
		USceneComponent* baseScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		UParticleSystemComponent* PortalSystem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		USphereComponent* BuildRadiusCol;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		USphereComponent* EnemyInfluenceRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		UCapsuleComponent* PortalMinimiumRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		UDecalComponent* ShowBuildable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToCareAbout;
//PROPERTY(EditAnywhere, BlueprintReadOnly)
//float currentBuildRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float maxBuildRadius = 400;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float growSpeed;
};
