// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyBaseClass.h"
#include "TankStateCpp.h"
//#include "EnemyEventHandler.h"
#include "LaserOrbCpp.generated.h"

UCLASS()
class TANKTRYCPP_API ALaserOrbCpp : public AEnemyBaseClass
{
	GENERATED_BODY()

		DECLARE_DELEGATE_OneParam(ChildIsBeingHurt, struct FHitDir)

public:
	// Sets default values for this actor's properties
	ALaserOrbCpp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

public:
	//UPROPERTY(EditAnywhere)
	//class ULaserSentryPMC* OurMovementComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
	//	float speed = 200;

	////Old stuff

	ChildIsBeingHurt HurtDele;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AiEssential)
		ATankStateCpp *TankState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		TEnumAsByte<ECollisionChannel> laserBlockingChannel;
	//TArray<TEnumAsByte<EObjectTypeQuery>> laserBlockingObjects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		TSubclassOf<UDamageType> damageTypeGiven;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float modelWidth;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		//UBillboardComponent* firePoint;

	//About Enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
		float health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
		float damagePerShot = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = EnemyEssential)
		float damageFalloff = 2500;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
	//	float fireRate = 120;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
	//	float dodgeChanceToDmgRatio = 15;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
	//	float dodgeCooldown = 5;
	//float currentDodgeTime = dodgeCooldown;
public:
	//virtual UPawnMovementComponent* GetMovementComponent() const override;
	//UFUNCTION(BlueprintImplementableEvent, Category = AiMovement)
	//	void WrongPlace(bool direction);
	//UFUNCTION(BlueprintImplementableEvent, Category = AiMovement)
	//	void WrongHeight(bool direction);
	//UFUNCTION(BlueprintImplementableEvent, Category = AiMovement)
	//	void StartDodging(FVector direction);
	UFUNCTION(BlueprintImplementableEvent, Category = AiEvents)
		void LookForNewTarget();
	UFUNCTION(BlueprintImplementableEvent, Category = AiEvents)
		void LaserEffects();
	UFUNCTION(BlueprintImplementableEvent, Category = EnemyEssential)
		void DeathEfx();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot(AActor* target);
private:
	//FTimerHandle shootingTimer;

	//UFUNCTION(BlueprintCallable, Category = Rip)
	//void DoesThisWorkTwice();
};
