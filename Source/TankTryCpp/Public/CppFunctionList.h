// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HoverTank.h"
//#include "CppFunctionList.generated.h"
/**
 *
 */
class UCppFunctionList
{
		/*	UFUNCTION(BlueprintCallable, Category = "OnlyCpp")
			static void EditDisplaySize(UPARAM(ref) UDecalComponent *hello, float sizeToSetTo);
		UFUNCTION(BlueprintCallable, Category = "OnlyCpp")
			static void SpawnDecalCorrectly(UMaterialInterface* inDecal, FVector decalSize, USceneComponent* parentComp, FVector location, FRotator rotation);*/

			//UFUNCTION(BlueprintCallable, Category = doesthiswork)
			//	static void Hahaha();
public:
	static void PrintVector(FVector input, float timeOnScreen = 5);
	static void PrintRotator(FRotator input);
	static void PrintString(FString txt);
	static void PrintString(FString txt, float input);
	static void PrintBool(bool inBool);
	static FVector Clamp3Numbers(FVector input, float maxX, float maxY, float maxZ, float minX, float minY, float minZ);
	static float CosineGraph(float Ampitude, float waveLength, float X);
	static FRotator SpreadRotator(FRotator inRot, float spreadAmt);
	static FRotator RandomRotator(bool roll);
	static bool GetPlayerPawnCasted(AHoverTank*& OutTank, UWorld* world);
	static bool StringToVector(FString inString, FVector& outVec);
	static FString VectorToString(FVector inVec);

	
//	static FVector FindEmptyOrbitRadiusPos(ADonNavigationManager* navManager, FVector target, FVector orbitor, float distance);
	//static bool IsWithinDistance(float a, float b, float tolerance);
};
