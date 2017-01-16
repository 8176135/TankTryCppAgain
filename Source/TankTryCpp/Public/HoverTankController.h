// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TankStateCpp.h"
#include "BaseTurretHUD.h"
#include "BaseTurret.h"
#include "SpecCamera.h"
#include "StructStorage.h"
#include "TurretPlaceholderBase.h"
#include "HoverTankController.generated.h"

/**
 *
 */
UCLASS()
class TANKTRYCPP_API AHoverTankController : public APlayerController
{
	GENERATED_BODY()
public:
	AHoverTankController(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* inPawn) override;
	virtual void SetupInputComponent() override;

	//Input Axis
	//void MoveForward(float AxisValue);
	//void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	//Input Actions
	void BeginFiring();
	void EndFiring();
	void ZoomIn();
	void ZoomOut();
	void RequestRespawn();
	void JumpTurret();
	void RequestTurretBuild();
	void ShowHpStats();
	void HideHpStats();
	void ExitGame();

	UFUNCTION()
		void MovementIsCompleted();
	UFUNCTION()
		void SubjectTurretIsDestroyed(AActor* DestroyedActor);
	UFUNCTION()
		void OnControlledTurretDamaged(FHitDir hitDamageInfo);
	UFUNCTION()
		void OnControlledTurretHitEnemy(float DamageAmount);
	UFUNCTION()
		bool BuildTurret(FVector buildLocation);
public:
	UPROPERTY(BlueprintReadOnly)
		class ABaseTurret* controlledPawn;
	class ASpecCamera* transitionCamera;
	class ATurretPlaceholderBase* turretHologram;
	UPROPERTY(BlueprintReadOnly)
		class ATankStateCpp* tankState;
	UPROPERTY()
		TArray<ABaseTurret*> allTurretsSpawned;
	UPROPERTY()
		TArray<FHitDir> hitDirections;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABaseTurret> unitToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ASpecCamera> specCamToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ATurretPlaceholderBase> placeholderTurret;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		TArray<TEnumAsByte<EObjectTypeQuery>> eyeBlockingObjects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
		float defaultPOV = 90;
protected:
	class ABaseTurretHUD* actualHUD;
private:
	bool allowJump = true;
	bool prepToBuild = false;
	ABaseTurret* possesionElict;
};
