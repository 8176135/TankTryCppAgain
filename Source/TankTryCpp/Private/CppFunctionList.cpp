// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"


//UCppFunctionList::UCppFunctionList(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}

//void UCppFunctionList::EditDisplaySize(UPARAM(ref) UDecalComponent *hello, float sizeToSetTo)
//{
//	if (IsValid(hello))
//	{
//		hello->FadeScreenSize = sizeToSetTo;
//	}
//}

//void UCppFunctionList::SpawnDecalCorrectly(UMaterialInterface *inDecal, FVector decalSize, USceneComponent* parentComp, FVector location, FRotator rotation)
//{
//	UGameplayStatics::SpawnDecalAttached(inDecal, decalSize, parentComp, NAME_None, location, rotation, EAttachLocation::KeepWorldPosition, 60);
//}
//
//void UCppFunctionList::Hahaha()
//{
//	
//}

void UCppFunctionList::PrintVector(FVector input, float timeOnScreen)
{
	UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f, Z: %f"), input.X, input.Y, input.Z);
	GEngine->AddOnScreenDebugMessage(-1, timeOnScreen, FColor::Cyan, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), input.X, input.Y, input.Z));
}
void UCppFunctionList::PrintRotator(FRotator input)
{
	UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f, Z: %f"), input.Roll, input.Pitch, input.Yaw);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), input.Roll, input.Pitch, input.Yaw));
}
void UCppFunctionList::PrintString(FString txt)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *(txt));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, (txt));
}

void UCppFunctionList::PrintString(FString txt, float input)
{
	PrintString(FString::Printf(*txt, input));
}

void UCppFunctionList::PrintBool(bool inBool)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), inBool ? "True" : "False");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("%s"), inBool ? "True" : "False"));
}

FVector UCppFunctionList::Clamp3Numbers(FVector input, float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
{
	input.X = FMath::Clamp(input.X, minX, maxX);
	input.Y = FMath::Clamp(input.Y, minY, maxY);
	input.Z = FMath::Clamp(input.Z, minZ, maxZ);
	return input;
}

float UCppFunctionList::CosineGraph(float Ampitude, float waveLength, float X)
{
	float cosineAngle = FMath::Cos(waveLength * X);
	return (cosineAngle * Ampitude + FMath::Abs(Ampitude));
}

FRotator UCppFunctionList::SpreadRotator(FRotator inRot, float spreadAmt)
{
	inRot.Roll = inRot.Roll - spreadAmt / 2 + FMath::FRand() * spreadAmt;
	inRot.Yaw = inRot.Yaw - spreadAmt / 2 + FMath::FRand() * spreadAmt;
	inRot.Pitch = inRot.Pitch - spreadAmt / 2 + FMath::FRand() * spreadAmt;
	return inRot;
}

FRotator UCppFunctionList::RandomRotator(bool roll)
{
	FRotator outRot;
	if (roll)
	{
		outRot.Roll = FMath::FRandRange(0, 360);
	}
	outRot.Pitch = FMath::FRandRange(0, 360);
	outRot.Yaw = FMath::FRandRange(0, 360);
	return outRot;
}

bool UCppFunctionList::GetPlayerPawnCasted(AHoverTank*& OutTank, UWorld* world)
{
	APawn* tempPlayer = UGameplayStatics::GetPlayerPawn(world, 0);
	if (IsValid(tempPlayer))
	{
		OutTank = Cast<AHoverTank>(tempPlayer);
		return true;
	}
	else
	{
		OutTank = NULL;
		return false;
	}
}

bool UCppFunctionList::StringToVector(FString inString, FVector& outVec)
{
	//X=%3.3f Y=%3.3f Z=%3.3f
//	int xInd;
//	int yInd;
//	int zInd;

	char* cAry = TCHAR_TO_ANSI(*inString);
	FString xValue = FString();
	FString yValue = FString();
	FString zValue = FString();

	int theSwitcher = 0;

	for (int i = 0; i < inString.Len(); ++i)
	{
		if (cAry[i] == ' ')
		{
			theSwitcher++;
			continue;
		}

		if (theSwitcher == 0)
		{
			xValue.AppendChar(cAry[i]);
		}
		else if (theSwitcher == 1)
		{
			yValue.AppendChar(cAry[i]);
		}
		else if (theSwitcher == 2)
		{
			zValue.AppendChar(cAry[i]);
		}
		else
		{
			break;
		}

	}
	if (theSwitcher == 2)
	{
		outVec = FVector(FCString::Atof(*xValue), FCString::Atof(*yValue), FCString::Atof(*zValue));
		return true;
	}
	else
	{
		return false;
	}


	//if (inString.FindChar('X', xInd) && inString.FindChar('Y', yInd) && inString.FindChar('Z', zInd))
	//{

	//	for (int i = xInd; i < yInd - 1; ++i)
	//	{
	//		xValue.AppendChar(cAry[i]);
	//	}
	//	outVec = cAry[xInd]
	//}
	//else
	//{
	//	return false;
	//}


}

FString UCppFunctionList::VectorToString(FVector inVec)
{
	return FString::Printf(TEXT("%d %d %d"), FMath::RoundToInt(inVec.X), FMath::RoundToInt(inVec.Y), FMath::RoundToInt(inVec.Z));
}


//FVector UCppFunctionList::FindEmptyOrbitRadiusPos(ADonNavigationManager* navManager, FVector target, FVector orbitor, float distance)
//{
//	FVector direction = (orbitor - target).GetSafeNormal();
//	if (direction == FVector::ZeroVector)
//	{
//		return FVector();
//	}
//	FRotator startingRot = direction.ToOrientationRotator();
//	for (int i = 0; i < 360; i += 3)
//	{
//		FRotator actualRot = startingRot.Add(0, i, 0);
//
//		FVector currentTarget = target + (actualRot.Vector() * distance);
//		FDonNavigationVoxel* checkData = navManager->VolumeAt(currentTarget);
//		if (checkData->CanNavigate())
//		{
//			return currentTarget;
//		}
//		else
//		{
//			//PrintString(TEXT("WTF"));
//		}
//	}
//	return FVector();
//}