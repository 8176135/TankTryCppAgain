// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "BlueprintStaticsActor.h"


// Sets default values
ABlueprintStaticsActor::ABlueprintStaticsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
}

void ABlueprintStaticsActor::RecalculateRefelction(UReflectionCaptureComponent* refCapture, UTextureCube* newCubemap)
{
	refCapture->Cubemap = newCubemap;
	refCapture->SetCaptureIsDirty();
}