// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "TankStateCpp.h"
#include "UnrealNetwork.h"

void ATankStateCpp::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ATankStateCpp, AmmoCurrent);
	DOREPLIFETIME(ATankStateCpp, AvailableAmmo);
}


