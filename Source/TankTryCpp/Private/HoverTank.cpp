// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "HoverTank.h"
#include "CppFunctionList.h"


// Sets default values
AHoverTank::AHoverTank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	baseSphereComp = CreateDefaultSubobject<USphereComponent>("BaseSphere");
	baseSphereComp->SetSphereRadius(54.011185);
	baseSphereComp->SetCollisionProfileName("NewPawn");
	baseSphereComp->SetLinearDamping(2);
	baseSphereComp->SetSimulatePhysics(true);
	SetRootComponent(baseSphereComp);

	mainPlayerSke = CreateDefaultSubobject<USkeletalMeshComponent>("MainPlayerSkeleton");
	mainPlayerSke->bGenerateOverlapEvents = true;
	mainPlayerSke->SetCollisionProfileName("HitBox");
	mainPlayerSke->AttachToComponent(baseSphereComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	eyeCam = CreateDefaultSubobject<UCameraComponent>("EyeCam");
	eyeCam->ComponentTags.Add("MainCamera");
	eyeCam->AttachToComponent(mainPlayerSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "EyeSocket");
	eyeCam->SetRelativeRotation(FRotator(0, 0, -90));
	//mainPlayerSke = Cast<USkeletalMeshComponent>(GetComponentByClass(TSubclassOf<USkeletalMeshComponent>()));

	//TArray<UActorComponent*> cameraStuff = GetComponentsByTag(TSubclassOf<UActorComponent>(), "MainCamera");
	//UCppFunctionList::PrintString("eeee ---> %d", cameraStuff.Num());
	//if (cameraStuff.Num() > 0)
	//{
	//	eyeCam = Cast<UCameraComponent>(cameraStuff[0]);
	//}
}

// Called when the game starts or when spawned
void AHoverTank::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHoverTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHoverTank::SetupPlayerInputComponent(class UInputComponent* inInputComponent)
{
	Super::SetupPlayerInputComponent(inInputComponent);

}

float AHoverTank::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FHitResult hitRes;
	FVector impulseDir;
	DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);
	if (!IsDamageble)
	{
		return 0;
	}

	health = FMath::Clamp(health - Damage, 0.0f, 100.0f);
	if (health == 0 && GetLifeSpan() == 0)
	{
		SetLifeSpan(0.1f);
	}
	return Damage;
}

void AHoverTank::Destroyed()
{
	//UGameplayStatics::SpawnEmitterAtLocation()
}

//void AHoverTank::UpdatePostProcessSettings(float deltaTime, UPARAM(ref) FPostProcessSettings& currentPPS)
//{
//	{
//		float x = FMath::Acos(currentPPS.WhiteTint);
//		currentPPS.WhiteTint = cos(x + deltaTime);
//		//UCppFunctionList::PrintString(FString::Printf(TEXT("fefe --> w %f --)) %d"), currentPPS.WhiteTint));
//	//	UCppFunctionList::PrintString(FString::Printf(TEXT("fefe --> w %d --)) %d"), currentPPS.bOverride_WhiteTint, currentPPS.bOverride_WhiteTemp));
//	}
//}

void AHoverTank::EditDisplaySize(UPARAM(ref) UDecalComponent *hello, float sizeToSetTo)
{
	if (IsValid(hello))
	{
		hello->FadeScreenSize = sizeToSetTo;
	}
}

//FVector AHoverTank::GetClosestLocAroundTarget(ADonNavigationManager* navManager,FVector target, FVector orbitor, float distance)
//{
//	return UCppFunctionList::FindEmptyOrbitRadiusPos(navManager, target, orbitor, distance);
//}

