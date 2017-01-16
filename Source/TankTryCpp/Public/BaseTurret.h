// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TankStateCpp.h"
#include "Animation/AnimInstance.h"
#include "StructStorage.h"
#include "BaseTurret.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FTurretDamagedSignature, FHitDir, DamageInfo);
DECLARE_DYNAMIC_DELEGATE_OneParam(FTargetHitSignature, float, DamageAmount);

UCLASS()
class TANKTRYCPP_API ABaseTurret : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseTurret();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	// Called to bind functionality to input
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual float GetTurretCost() const;

	void BeginFiring();
	void StopFiring();
	void FireGun();
	float GetSpreadVal() const;

//	virtual void BeginDestroy() override;
	virtual void ConfigureSpawn();
	UFUNCTION(BlueprintImplementableEvent, Category = Components)
		void SetWidgetVisibility(bool show);
public:
	UPROPERTY(BlueprintReadOnly, Category = Components)
		USceneComponent* sceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		USkeletalMeshComponent* mainTurretSke;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		UCameraComponent* eyeCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		UParticleSystemComponent* bulletPartic;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		UParticleSystemComponent* muzzleFlashPartic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float damagePerShot = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		TSubclassOf<UDamageType> dmgType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float maxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float health = maxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Animation)
		float turretYaw = 0;
	UPROPERTY(BlueprintReadOnly, Category = Animation)
		float turretTruePitch = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		UAnimMontage* gunFireMont;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		float fireRPM = 250;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		float gunHeatPerShot = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		float gunCooldownSpeed = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		float maxInaccuracy = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		TEnumAsByte<ECollisionChannel> bulletBlockingChannel;
		//TArray<TEnumAsByte<EObjectTypeQuery>> bulletBlockers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
		UParticleSystem* bulletHitPartic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
		UMaterialInterface* bulletHitDecal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
		UParticleSystem* destructionExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
		TSubclassOf<UCameraShake> destructionCamShake;

	bool invincibility = false;

	FTurretDamagedSignature turretDmgDele;
	FTargetHitSignature targetHitDele;

	TArray<ABaseTurret*>* allTurrets;
protected:
	class ATankStateCpp* tankState;
	//FCollisionObjectQueryParams objQuery;
	FCollisionQueryParams colQuery;
	UAnimInstance* animationInst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float cost = 20;

	//OverridableFuncs
	virtual void ParticleEfx(FVector trueAimLoc);

private:
	FTimerHandle firingTimer;
	bool timerPausePend = false;
	float spreadXVal = 0;
};
