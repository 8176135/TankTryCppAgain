// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "HoverTank.generated.h"

UCLASS()
class TANKTRYCPP_API AHoverTank : public APawn
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Uti")
		void EditDisplaySize(UPARAM(ref) UDecalComponent * hello, float sizeToSetTo);
	// Sets default values for this pawn's properties
	AHoverTank();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essentials")
		float health = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extras")
		bool IsDamageble = true;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float TurretYaw = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float TruePitch = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* baseSphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* mainPlayerSke;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* eyeCam;

	

private:

	//UFUNCTION(BlueprintPure, meta=(DisplayName = "Get Closest Location Around Target", Category = "Utility"))
	//FVector GetClosestLocAroundTarget(ADonNavigationManager* navManager, FVector target, FVector orbitor, float distance);
};



