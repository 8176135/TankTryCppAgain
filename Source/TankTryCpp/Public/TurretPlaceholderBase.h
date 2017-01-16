// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TurretPlaceholderBase.generated.h"

UCLASS()
class TANKTRYCPP_API ATurretPlaceholderBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretPlaceholderBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Control")
		void SetVisibility(bool visible);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* sceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		UBoxComponent* colComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		USkeletalMeshComponent* turretPh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		UMaterialInterface* turretInstMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ColorCustomization)
		FLinearColor somethingInTheWay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ColorCustomization)
		FLinearColor clear;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool ableToSpawn;
	UMaterialInstanceDynamic* turretDynMat;



};
