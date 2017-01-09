// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "TankStateCpp.generated.h"

/**
 *
 */
UCLASS()
class TANKTRYCPP_API ATankStateCpp : public APlayerState
{
	GENERATED_BODY()

public:

	//static ATankStateCpp* theCorrectPlayerState;
	//Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int AmmoClipMax = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, Replicated)
		int AmmoCurrent = AmmoClipMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int StartingCarryAmmo = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, Replicated)
		int AvailableAmmo = StartingCarryAmmo - AmmoClipMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int MaxCarryAmmo = 120;

	//Boost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boost)
		float BoostFuelMax = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boost)
		float BoostFuel = BoostFuelMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boost)
		bool Boosted = false;

	//Basic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		float Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		bool AbleToRespawn = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		float RespawnTime = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		FVector AimLocation = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basics)
		FHitResult AimTHitRes;

	//Money
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resources)
		float Scrap = 0;


	//EnemyStrats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyState)
		float EnemyLevel = 0;

	
};
