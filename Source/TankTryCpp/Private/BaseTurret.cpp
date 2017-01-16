// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "BaseTurret.h"


// Sets default values
ABaseTurret::ABaseTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//colQuery = FCollisionQueryParams(NAME_None,false,);
	//colQuery.bTraceComplex = true;
	//colQuery.AddIgnoredActor(this);

	//objQuery = FCollisionObjectQueryParams();
	//for (TEnumAsByte<ECollisionChannel> blocker : bulletBlockers)
	//{
	//	objQuery.AddObjectTypesToQuery(blocker.GetValue());
	//}

	Tags.Add("Turret");

	//Components
	sceneComp = CreateDefaultSubobject<USceneComponent>("BaseSceneComp");
	SetRootComponent(sceneComp);

	mainTurretSke = CreateDefaultSubobject<USkeletalMeshComponent>("MainTurretSkeleton");
	mainTurretSke->AttachToComponent(sceneComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	animationInst = mainTurretSke->GetAnimInstance();

	bulletPartic = CreateDefaultSubobject<UParticleSystemComponent>("BulletTracerParticle");
	bulletPartic->bAutoActivate = false;

	muzzleFlashPartic = CreateDefaultSubobject<UParticleSystemComponent>("MuzzleFlashParticle");
	muzzleFlashPartic->bAutoActivate = false;
	muzzleFlashPartic->SecondsBeforeInactive = 0.5;

	eyeCam = CreateDefaultSubobject<UCameraComponent>("EyeCamera");

	if (mainTurretSke->DoesSocketExist("EyeSocket"))
	{
		eyeCam->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "EyeSocket");
		eyeCam->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, -90, 0));
	}
	if (mainTurretSke->DoesSocketExist("WrongEnd"))
	{
		bulletPartic->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "WrongEnd");
		muzzleFlashPartic->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "WrongEnd");
	}
}

// Called when the game starts or when spawned
void ABaseTurret::BeginPlay()
{
	tankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);

	animationInst = mainTurretSke->GetAnimInstance();
	GetWorldTimerManager().SetTimer(firingTimer, this, &ABaseTurret::FireGun, 60 / fireRPM, true);
	GetWorldTimerManager().PauseTimer(firingTimer);
	Super::BeginPlay();
}

// Called every frame
void ABaseTurret::Tick(float DeltaTime)
{
	spreadXVal = FMath::Clamp(spreadXVal - DeltaTime * PI * gunCooldownSpeed, 0.0f, PI);
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
//void ABaseTurret::SetupPlayerInputComponent(class UInputComponent* inputComp)
//{
//	Super::SetupPlayerInputComponent(inputComp);
//}

void ABaseTurret::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABaseTurret::OnConstruction(const FTransform& Transform)
{
	if (mainTurretSke->DoesSocketExist("EyeSocket"))
	{
		eyeCam->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "EyeSocket");
		eyeCam->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, -90, 0));
	}
	if (mainTurretSke->DoesSocketExist("WrongEnd"))
	{
		bulletPartic->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "WrongEnd");
		muzzleFlashPartic->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "WrongEnd");
	}
	mainTurretSke->RelativeLocation = GetActorUpVector() * -mainTurretSke->Bounds.BoxExtent.Z / 2;
}

float ABaseTurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (invincibility)
	{
		return 0;
	}
	FHitResult hitInfo;
	FVector impulseDir;
	DamageEvent.GetBestHitInfo(this, DamageCauser, hitInfo, impulseDir);
	FHitDir hitDamageInfo;
	hitDamageInfo.damage = DamageAmount;
	hitDamageInfo.dmgType = DamageEvent.DamageTypeClass;
	if (DamageEvent.GetTypeID() == FRadialDamageEvent::ClassID)
	{
		hitDamageInfo.radius = 20;
	}
	else if (DamageEvent.GetTypeID() == FPointDamageEvent::ClassID)
	{
		hitDamageInfo.hitDirection = impulseDir.Rotation() - eyeCam->GetForwardVector().Rotation();
	}
	else if (DamageEvent.GetTypeID() == FDamageEvent::ClassID)
	{
		hitDamageInfo.enviromental = true;
	}
	else
	{
		UCppFunctionList::PrintString("WTF!!!!!, TAKE DAMAGE NOT WORKING PROPERLY");
	}

	turretDmgDele.ExecuteIfBound(hitDamageInfo);
	health = FMath::Clamp(health - DamageAmount, 0.0f, maxHealth);

	if (health == 0 && GetLifeSpan() == 0)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, destructionExplosion, GetActorLocation(), UCppFunctionList::RandomRotator(true));
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), destructionCamShake, GetActorLocation(), 200, 300);
		SetLifeSpan(0.05f);
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseTurret::BeginFiring()
{
	GetWorldTimerManager().UnPauseTimer(firingTimer);
	timerPausePend = false;
	if (animationInst)
	{
		animationInst->Montage_Play(gunFireMont, fireRPM / 60, EMontagePlayReturnType::Duration, 0);
	}
}

void ABaseTurret::StopFiring()
{
	timerPausePend = true;
}

void ABaseTurret::FireGun()
{
	if (timerPausePend)
	{
		GetWorldTimerManager().PauseTimer(firingTimer);
		animationInst->Montage_Stop(0.1f, gunFireMont);
		timerPausePend = false;
		return;
	}
	FVector startingLoc = mainTurretSke->GetSocketLocation("EyeSocket");

	FVector deltaVector = tankState->AimLocation - startingLoc;
	FRotator aimingRotation = deltaVector.Rotation();
	float spreadAmount = UCppFunctionList::CosineGraph(-maxInaccuracy, 1, spreadXVal);
	FVector actualBulletDir = UCppFunctionList::SpreadRotator(aimingRotation, spreadAmount).Vector();
	actualBulletDir *= 9000;
	FVector endingLoc = startingLoc + actualBulletDir;

	FHitResult hitRes;
	bool hit = GetWorld()->LineTraceSingleByChannel(hitRes, startingLoc, endingLoc, bulletBlockingChannel, FCollisionQueryParams(NAME_None, false));
	if (hit && hitRes.IsValidBlockingHit())
	{
		if (IsValid(hitRes.GetActor()))
		{
			if (hitRes.GetActor()->ActorHasTag("Enemy"))
				targetHitDele.ExecuteIfBound(damagePerShot);
			else if (hitRes.GetActor()->ActorHasTag("Turret"))
			{
				animationInst->Montage_Stop(0, gunFireMont);
				return;
			}
			if (!animationInst->Montage_IsPlaying(gunFireMont))
			{
				animationInst->Montage_Play(gunFireMont, fireRPM / 60, EMontagePlayReturnType::Duration, 0);
			}
			UGameplayStatics::ApplyPointDamage(hitRes.GetActor(), damagePerShot, hitRes.TraceStart - hitRes.Location, hitRes, GetController(), this, dmgType);
		}
		if (hitRes.GetComponent())
		{
			UGameplayStatics::SpawnDecalAttached(bulletHitDecal, FVector(5, FMath::FRandRange(8, 10.5f), FMath::FRandRange(8, 10.5f)),
				hitRes.GetComponent(), NAME_None, hitRes.Location, hitRes.Normal.Rotation().Add(180, 0, FMath::FRand() * 360), EAttachLocation::KeepWorldPosition, 60);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletHitPartic, hitRes.Location, hitRes.Normal.Rotation());
		}
		ParticleEfx(hitRes.Location);
	}
	else
	{
		ParticleEfx(endingLoc);
	}
	spreadXVal = FMath::Clamp(spreadXVal + gunHeatPerShot, 0.0f, PI);

}

float ABaseTurret::GetSpreadVal() const
{
	return spreadXVal;
}

//void ABaseTurret::BeginDestroy()
//{
//	UGameplayStatics::SpawnEmitterAtLocation(this, destructionExplosion, GetActorLocation(), UCppFunctionList::RandomRotator(true));
//	Super::BeginDestroy();
//}

float ABaseTurret::GetTurretCost() const
{
	return cost;
}
void ABaseTurret::ConfigureSpawn()
{
	AddActorWorldOffset(FVector(0, 0, -mainTurretSke->RelativeLocation.Z));
}

void ABaseTurret::ParticleEfx(FVector trueAimLoc)
{
	muzzleFlashPartic->Activate(true);
	bulletPartic->SetWorldRotation((trueAimLoc - bulletPartic->GetComponentLocation()).Rotation());
	bulletPartic->Activate(true);
}
