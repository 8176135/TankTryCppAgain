// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BlueprintStaticsActor.generated.h"

UCLASS()
class TANKTRYCPP_API ABlueprintStaticsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlueprintStaticsActor();

	UFUNCTION(BlueprintCallable, Category = "UtilitiesMaybe")
		void RecalculateRefelction(UReflectionCaptureComponent* refCapture, UTextureCube* newCubemap);
	
};
