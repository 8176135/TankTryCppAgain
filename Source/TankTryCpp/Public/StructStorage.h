// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StructStorage.generated.h"
/**
 * 
 */
USTRUCT()
struct FHitDir {
	GENERATED_BODY()

		FRotator hitDirection;
	float lifetime = 2;
	float damage;
	float radius = 0;
	bool enviromental = false;
	TSubclassOf<UDamageType> dmgType;
};