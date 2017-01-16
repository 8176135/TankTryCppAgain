// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "BaseTurret.h"
#include "HoverTankController.h"
#include "BaseTurretHUD.generated.h"

/**
 *
 */
UCLASS()
class TANKTRYCPP_API ABaseTurretHUD : public AHUD
{
	GENERATED_BODY()
public:
	ABaseTurretHUD();
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	virtual void PostInitializeComponents() override;

	void UpdateHitMarker();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> PlayerStatsWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
		class UUserWidget* playerStatsWidgetInst;
	//Direct Draw
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
//		UMaterialInterface* crosshairInterface;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweaks)
//		float crosshairSize = 250;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweaks)
//		float crosshairSpread = 2.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
		UMaterialInterface* damagedHitInterface;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweaks)
		float hitMarkerSize = 250;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweaks)
		float hitMarkerMaxTime = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
		UMaterialInterface* hitEnemyInterface;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tweaks)
		float hitEnemySize = 250;
protected:
	float hitMarkerDisplayTimer;
	class AHoverTankController* actualController;
//	UPROPERTY()
	//	UMaterialInstanceDynamic* instCrosshair;
	UPROPERTY()
		UMaterialInstanceDynamic* instHitMarker;
};
