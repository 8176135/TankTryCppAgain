// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "LaserOrbCpp.h"
#include "StructStorage.h"
#include "CppFunctionList.h"

// Sets default values
ALaserOrbCpp::ALaserOrbCpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//OurMovementComponent = CreateDefaultSubobject<ULaserSentryPMC>(TEXT("LaserSentryCustomMC"));
	//OurMovementComponent->UpdatedComponent = RootComponent;
	//firePoint->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ALaserOrbCpp::BeginPlay()
{
	TankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	Super::BeginPlay();
}

// Called every frame
void ALaserOrbCpp::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

float ALaserOrbCpp::TakeDamage(float damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FHitResult hitRes;
	FVector impulseDir;
	DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);

	health -= damage;

	if (health <= 0)
	{
		SetLifeSpan(0.1f);
		DeathEfx();
	}
	else
	{
		if (HurtDele.IsBound())
		{
			FHitDir hitInfo;
			hitInfo.dmgType = DamageEvent.DamageTypeClass;
			hitInfo.damage = damage;
			HurtDele.Execute(hitInfo);
		}
		else
		{
			UCppFunctionList::PrintString(TEXT("Delegate not bound..?"));
		}
	}
	return Super::TakeDamage(damage, DamageEvent, EventInstigator, DamageCauser);
}

void ALaserOrbCpp::Shoot(AActor* target)
{
	FHitResult laserFireRes;
	bool isHit = GetWorld()->LineTraceSingleByChannel(laserFireRes, GetActorLocation() + GetActorForwardVector() * 40, target->GetActorLocation(),
		laserBlockingChannel, FCollisionQueryParams(FName(EName::NAME_None), false));
	if (!isHit)
	{
		UCppFunctionList::PrintString("Target collision not setup properly");
		return;
	}
	if (!IsValid(laserFireRes.GetActor()))
	{
		UCppFunctionList::PrintString("Shot target is not valid");
		return;
	}
	if (laserFireRes.GetActor()->GetUniqueID() == target->GetUniqueID())
	{
		float distance = (target->GetActorLocation() - GetActorLocation()).Size();
		float damage = UCppFunctionList::CosineGraph(damagePerShot, damageFalloff, distance);
		FVector hitDir = (laserFireRes.ImpactPoint - laserFireRes.TraceStart).GetSafeNormal();
		UGameplayStatics::ApplyPointDamage(target, damagePerShot, hitDir, laserFireRes, nullptr, this, damageTypeGiven);
	}
	else if (laserFireRes.GetActor()->ActorHasTag("Turret"))
	{
		float distance = (laserFireRes.GetActor()->GetActorLocation() - GetActorLocation()).Size();
		float damage = UCppFunctionList::CosineGraph(damagePerShot, damageFalloff, distance);
		FVector hitDir = (laserFireRes.ImpactPoint - laserFireRes.TraceStart).GetSafeNormal();
		UGameplayStatics::ApplyPointDamage(laserFireRes.GetActor(), damagePerShot, hitDir, laserFireRes, nullptr, this, damageTypeGiven);
	}
	LaserEffects();
}

//UPawnMovementComponent* ALaserOrbCpp::GetMovementComponent() const
//{
//	return OurMovementComponent;
//}

//void ALaserOrbCpp::TakingDamage(float Damage, FVector hitFrom)
//{
//	
//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Workz"));
//	if (FMath::FRand() < Damage / dodgeChanceToDmgRatio && currentDodgeTime > dodgeCooldown)
//	{
//		currentDodgeTime = 0;
//		if (FMath::RandBool())
//		{
//			StartDodging(hitFrom.GetSafeNormal().RotateAngleAxis(90, FVector::UpVector));
//		}
//		else
//		{
//			StartDodging(hitFrom.GetSafeNormal().RotateAngleAxis(-90, FVector::UpVector));
//		}
//	}
//}

