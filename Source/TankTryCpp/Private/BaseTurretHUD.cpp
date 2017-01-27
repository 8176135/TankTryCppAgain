// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "StructStorage.h"
#include "HoverTankController.h"
#include "BaseTurretHUD.h"

ABaseTurretHUD::ABaseTurretHUD()
{

}

void ABaseTurretHUD::BeginPlay()
{
	actualController = Cast<AHoverTankController>(PlayerOwner);
	if (PlayerStatsWidget)
	{
		playerStatsWidgetInst = CreateWidget<UUserWidget>(PlayerOwner, PlayerStatsWidget);
		if (playerStatsWidgetInst)
		{
			playerStatsWidgetInst->AddToViewport();
		}
		else
		{
			UCppFunctionList::PrintString("UMG didnt load properly");
			
		}
	}
	Super::BeginPlay();
}

void ABaseTurretHUD::DrawHUD()
{
	Super::DrawHUD();
	if (actualController->controlledPawn)
	{
		//float inaccuracy = UCppFunctionList::CosineGraph(-actualController->controlledPawn->maxInaccuracy, 1, actualController->controlledPawn->GetSpreadVal());
		//instCrosshair->SetScalarParameterValue("Spread", inaccuracy / crosshairSpread);
		
		//DrawMaterialSimple(instCrosshair, Canvas->SizeX / 2 - crosshairSize / 2, Canvas->SizeY / 2 - crosshairSize / 2, crosshairSize, crosshairSize);
		for (FHitDir direction : actualController->hitDirections)
		{
			if (direction.enviromental)
			{
				
				continue;
			}
			UMaterialInstanceDynamic* dynMat = UMaterialInstanceDynamic::Create(damagedHitInterface, this);
			dynMat->SetScalarParameterValue("Rotation", FMath::DegreesToRadians(-direction.hitDirection.Yaw - 90));
			DrawMaterialSimple(dynMat, Canvas->SizeX / 2 - hitMarkerSize / 2, Canvas->SizeY / 2 - hitMarkerSize / 2, hitMarkerSize, hitMarkerSize);
		}
	}
	hitMarkerDisplayTimer = FMath::Max(hitMarkerDisplayTimer - RenderDelta, 0.0f);
	if (hitMarkerDisplayTimer > 0)
	{
		instHitMarker->SetScalarParameterValue("Spread", FMath::Lerp(-0.6f, -1.2f, hitMarkerDisplayTimer / hitMarkerMaxTime));
		DrawMaterialSimple(instHitMarker, Canvas->SizeX / 2 - hitEnemySize / 2, Canvas->SizeY / 2 - hitEnemySize / 2, hitEnemySize, hitEnemySize);
		
	}
}

void ABaseTurretHUD::PostInitializeComponents()
{
//	if (crosshairInterface)
//		instCrosshair = UMaterialInstanceDynamic::Create(crosshairInterface, this);
	if (hitEnemyInterface)
		instHitMarker = UMaterialInstanceDynamic::Create(hitEnemyInterface, this);
	Super::PostInitializeComponents();
}

void ABaseTurretHUD::UpdateHitMarker()
{
	hitMarkerDisplayTimer = hitMarkerMaxTime;
}
