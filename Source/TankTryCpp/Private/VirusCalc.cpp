// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "EnemyBaseClass.h"
#include "TankStateCpp.h"
#include "CppFunctionList.h"
#include "VirusCalc.h"

//#include "Virus"


// Sets default values
AVirusCalc::AVirusCalc()
{
	//VariableSetup
	PrimaryActorTick.bCanEverTick = true;
	FAttachmentTransformRules tempATR(EAttachmentRule::KeepRelative, false);
	vOQP = FCollisionObjectQueryParams();
	for (auto collisionChannel : ObstacleQueryChannels)
		vOQP.AddObjectTypesToQuery(collisionChannel);

	//ComponentSetup
	sceneComp = CreateDefaultSubobject<USceneComponent>("TheRootOfAllEvil");
	billboardMarker = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard Marker"));
	SetRootComponent(sceneComp);
	billboardMarker->AttachToComponent(RootComponent, tempATR);

	unifiedPPSettings.bOverride_WhiteTint = 1;
	unifiedPPSettings.bOverride_LensFlareIntensity = 1;
	unifiedPPSettings.bOverride_BloomIntensity = 1;
}

// Called when the game starts or when spawned
void AVirusCalc::BeginPlay()
{
	vColShape = FCollisionShape::MakeBox(FVector(BoxSize / 2 - 1, BoxSize / 2 - 1, BoxSize / 2 - 1));
	trueTankState = Cast<ATankStateCpp>(GetWorld()->GetFirstPlayerController()->PlayerState);
	attackLocations.Add(FVector(0, 0, 0));
	GetWorld()->GetTimerManager().SetTimer(spawningTimer, this, &AVirusCalc::SpawningTimeElapsed, timerLength, false);
	for (TActorIterator<ADonNavigationManager> It(GetWorld(), ADonNavigationManager::StaticClass()); It; ++It)
	{
		navManager = *It;
		break;
	}
	Super::BeginPlay();
}

// Called every frame
void AVirusCalc::Tick(float DeltaTime)
{
	storage.UpdateValues(DeltaTime);
	unifiedPPSettings.WhiteTint = FMath::Pow(FMath::Cos(FMath::Pow(FMath::Sin(storage.WhiteTint), 2)), 7) * 2 - 1;
	unifiedPPSettings.LensFlareIntensity = -FMath::Cos(storage.LensFlareIntensity * 2) / 2 + 1;
	unifiedPPSettings.BloomIntensity = FMath::Cos(storage.BloomIntensity) * 3 + 4;

	TArray<AActor*> allOverlappers = TArray<AActor*>();

	for (TPair<FVector, FVirusPart> data : allVirusData)
	{
		if (data.Value.dead)
		{
			continue;
		}
		data.Value.PPComponent->Settings = unifiedPPSettings;
		TArray<AActor*> tempActor;
		data.Value.meshComponent->GetOverlappingActors(tempActor);
		int currentSlack = allOverlappers.GetSlack();
		allOverlappers.Reserve(tempActor.Num() - currentSlack);
		for (AActor* actor : tempActor)
		{
			allOverlappers.AddUnique(actor);
		}
		if (data.Value.meshComponent->RelativeScale3D != FVector(1))
		{
			TArray<AActor*> influencers;
			float influencerPower = 0;
			data.Value.EnemyInfluenceVolume->GetOverlappingActors(influencers);
			for (AActor* influencer : influencers)
			{
				if (influencer->ActorHasTag("Enemy"))
				{
					influencerPower += Cast<AEnemyBaseClass>(influencer)->GetEnemyInfluence();
				}
			}
			influencers.Empty();
			FVector newScale = UCppFunctionList::Clamp3Numbers((DeltaTime * data.Value.numOfFullSizedNeighbours * FVector(spawnSpeed) * (1 - influencerPower) + data.Value.meshComponent->RelativeScale3D), 1, 1, 1, 0, 0, 0);
			data.Value.meshComponent->SetRelativeScale3D(newScale);
			if (!data.Value.blocked && newScale == FVector(1))
			{
				CheckForNeighbours(data.Key, true);
				QueueVirusesToSpawn(data.Key);
			}
		}
		else
		{
			trueTankState->Scrap += DeltaTime * scrapPerBlockPerSecond;
			allVirusData[data.Key].lifetime -= DeltaTime;
			if (data.Value.lifetime <= 0)
			{
				KillVirus(data.Key);
			}
			else if (data.Value.lifetime < data.Value.fadingTime)
			{
				if (!data.Value.fading)
				{
					allVirusData[data.Key].fading = true;
					data.Value.meshComponent->SetMaterial(0, fadingMaterial);
				}
				data.Value.PPComponent->BlendWeight = data.Value.lifetime * 0.7 / data.Value.fadingTime;
				data.Value.meshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), data.Value.lifetime / data.Value.fadingTime);
			}
		}
	}
	if (allOverlappers.Num() > 0)
	{
		for (AActor* actor : allOverlappers)
		{
			if (actor->ActorHasTag("Enemy"))
			{
				UGameplayStatics::ApplyDamage(actor, DeltaTime * dmgPerSecond, NULL, this, virusDmgTp);
				UCppFunctionList::PrintString("DamageApplied");
			}
			else if (actor->ActorHasTag("Turret"))
			{
				UGameplayStatics::ApplyDamage(actor, DeltaTime * dmgPerSecond, NULL, this, virusDmgTp);
			}
		}
	}
	Super::Tick(DeltaTime);
}

void AVirusCalc::OverlapBegins(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor->ActorHasTag("Enemy"))
	//{
	//	UCppFunctionList::PrintVector(HitComp->RelativeLocation);//FVector(HitComp->RelativeLocation.X / BoxSize, HitComp->RelativeLocation.Y / BoxSize, HitComp->RelativeLocation.Z / BoxSize));
	//}
}

void AVirusCalc::OverlapEnds(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//APawn* locPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//if (IsValid(locPlayer))
	//{
	//	if (OtherActor->GetUniqueID() == locPlayer->GetUniqueID())
	//	{
	//		numOfCompBeingOverlapped--;
	//	}
	//}
	//else
	//{
	//	numOfCompBeingOverlapped = 0;
	//}
}

void AVirusCalc::SpawningVirus(FVector curLoc)
{
	FAttachmentTransformRules tempATR(EAttachmentRule::KeepRelative, false);
	//curloc * box = (y - actloc)/box
	FVector realLoc = FVector((curLoc.X * BoxSize) + GetActorLocation().X, (curLoc.Y * BoxSize) + GetActorLocation().Y, (curLoc.Z * BoxSize) + GetActorLocation().Z);
	TArray<FOverlapResult> overLapResults;
	GetWorld()->OverlapMultiByObjectType(overLapResults, realLoc, FQuat::Identity, vOQP, vColShape, FCollisionQueryParams(FName("VirusSpreading"), false));

	UStaticMeshComponent* newStaticMeshComp = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Virus Part - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	newStaticMeshComp->RegisterComponent();
	newStaticMeshComp->AttachToComponent(RootComponent, tempATR);
	newStaticMeshComp->SetRelativeScale3D(FVector(1, 1, 1));
	newStaticMeshComp->SetWorldLocation(realLoc);
	newStaticMeshComp->SetStaticMesh(boxMesh);
	newStaticMeshComp->SetCollisionProfileName(FName("TemporaryPathBlock"));

	bool validity;
	navManager->ChangeCollisionAtMesh(newStaticMeshComp, false, true, validity, NAME_None, false);

	newStaticMeshComp->SetRelativeScale3D(FVector(0, 0, 0));
	newStaticMeshComp->SetCollisionProfileName(FName("VirusCol"));

	UBoxComponent* PPCVolume = NewObject<UBoxComponent>(this, FName(*FString::Printf(TEXT("BoxVol - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	PPCVolume->RegisterComponent();
	PPCVolume->SetBoxExtent(FVector(BoxSize / 2, BoxSize / 2, BoxSize / 2));
	PPCVolume->AttachToComponent(newStaticMeshComp, tempATR);
	PPCVolume->SetCollisionProfileName(FName("PPCol"));
	UPostProcessComponent* newPPC = NewObject<UPostProcessComponent>(this, FName(*FString::Printf(TEXT("PPC Sub - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	newPPC->RegisterComponent();
	newPPC->AttachToComponent(PPCVolume, tempATR);
	newPPC->Priority = 2;
	newPPC->BlendWeight = 0.7f;
	newPPC->bUnbound = false;

	USphereComponent* EnemyInfluenceVol = NewObject<USphereComponent>(this, FName(*FString::Printf(TEXT("EIVol - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	EnemyInfluenceVol->RegisterComponent();
	EnemyInfluenceVol->SetSphereRadius(enemyInfluenceRadius);
	EnemyInfluenceVol->SetCollisionProfileName("EnemyFinder");
	EnemyInfluenceVol->AttachToComponent(RootComponent, tempATR);
	EnemyInfluenceVol->SetWorldLocation(realLoc);
	//EnemyInfluenceVol->OnComponentBeginOverlap.AddDynamic(this, &AVirusCalc::OverlapBegins);
	//EnemyInfluenceVol->OnComponentEndOverlap.AddDynamic(this, &AVirusCalc::OverlapEnds);
	//navManager->UpdateCollisionAtMesh(newStaticMeshComp, validity, NAME_None, true);
	allVirusData.Add(curLoc, FVirusPart(realLoc, overLapResults.Num() != 0, newStaticMeshComp, newPPC, PPCVolume, EnemyInfluenceVol, totalLifetime, fadingTime));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), sparksPS, realLoc, FRotator(FMath::FRandRange(-60, 60), FMath::FRandRange(-180, 180), FMath::FRandRange(-180, 180)));
	CheckForNeighbours(curLoc, false);
}

void AVirusCalc::QueueVirusesToSpawn(FVector attackerLoc)
{
	for (FVector direction : allDirections)
	{
		FVector checkingLocation = attackerLoc + direction;
		if (!allVirusData.Contains(checkingLocation))
		{
			SpawningVirus(checkingLocation);
		}
	}
}

void AVirusCalc::KillVirus(FVector deadLoc)
{
	bool validity;
	navManager->ChangeCollisionAtMesh(allVirusData[deadLoc].meshComponent, true, true, validity, NAME_None, true);
	allVirusData[deadLoc].lifetime = 0;
	allVirusData[deadLoc].meshComponent->SetVisibility(false);
	allVirusData[deadLoc].meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	allVirusData[deadLoc].PPComponent->bEnabled = false;
	allVirusData[deadLoc].PPVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	allVirusData[deadLoc].dead = true;
	//FTimerHandle tempHandle;
	//FTimerDelegate ClearBlockDel = FTimerDelegate::CreateUObject(this, &AVirusCalc::ClearVirusNavBlock, deadLoc);
	//GetWorldTimerManager().SetTimer(tempHandle, ClearBlockDel, totalLifetime * 2.01f, false);

	//navManager->UpdateCollisionAtMesh(allVirusData[deadLoc].meshComponent, validity, NAME_None, true);
}

void AVirusCalc::CheckForNeighbours(FVector blockToCheck, bool invert)
{
	if (!allVirusData.Contains(blockToCheck))
	{
		UCppFunctionList::PrintString("Neighbour not in list");
		return;
	}
	if (!invert)
	{
		allVirusData[blockToCheck].numOfFullSizedNeighbours = 0;
		for (FVector direction : allDirections)
		{
			FVector checkingLocation = blockToCheck + direction;
			if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead
				&& !allVirusData[checkingLocation].blocked && allVirusData[checkingLocation].meshComponent->RelativeScale3D == FVector(1))
			{
				allVirusData[blockToCheck].numOfFullSizedNeighbours++;
			}
		}
	}
	else if (allVirusData[blockToCheck].meshComponent->RelativeScale3D == FVector(1))
	{
		for (FVector direction : allDirections)
		{
			FVector checkingLocation = blockToCheck + direction;
			if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead)
			{
				allVirusData[checkingLocation].numOfFullSizedNeighbours++;
			}
		}
	}
	else
	{
		UCppFunctionList::PrintString("Passed a block that is not full for an inverse check");
		return;
	}
}

void AVirusCalc::ForceUpdate()
{
	for (auto data : allVirusData)
	{
		if (!data.Value.dead)
		{
			if (data.Value.meshComponent->RelativeScale3D != FVector(1))
			{
				allVirusData[data.Key].numOfFullSizedNeighbours = 0;
				for (FVector direction : allDirections)
				{
					FVector checkingLocation = data.Key + direction;
					if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead
						&& !allVirusData[checkingLocation].blocked && allVirusData[checkingLocation].meshComponent->RelativeScale3D == FVector(1))
					{
						allVirusData[data.Key].numOfFullSizedNeighbours++;
					}
				}
			}
			else if (!data.Value.blocked)
			{
				QueueVirusesToSpawn(data.Key);
			}
		}
	}
}

void AVirusCalc::ClearVirusNavBlock(FVector blockToClear)
{
	//	bool validity;
	//	navManager->UpdateCollisionAtMesh(allVirusData[blockToClear].meshComponent, validity, NAME_None, true);
}

void AVirusCalc::SpawningTimeElapsed()
{
	SpawningVirus(FVector(0, 0, 0));
	allVirusData[FVector(0, 0, 0)].numOfFullSizedNeighbours++;
	//if (attackLocations.Num() > 0)
	//{
	//	if (allVirusData.Contains(attackLocations[0]) && !allVirusData[attackLocations[0]].dead)
	//	{
	//		attackLocations.RemoveAt(0);
	//		SpawningTimeElapsed();
	//		return;
	//	}
	//	FVector curLoc = attackLocations[0];
	//	attackLocations.RemoveAt(0);
	//	
	//	//if (overLapResults.Num() == 0)
	//	//{
	//	//	for (FVector direction : allDirections)
	//	//	{
	//	//		attackLocations.Add(curLoc + direction);
	//	//	}
	//	//}
	//	//if (attackLocations.Num() > 0)
	//	//{
	//	//	int influence = 0;
	//	//	for (FVector direction : allDirections)
	//	//	{
	//	//		if (allVirusData.Contains(attackLocations[0] + direction) && !allVirusData[attackLocations[0] + direction].blocked && !allVirusData[attackLocations[0] + direction].dead)
	//	//		{
	//	//			influence++;
	//	//		}
	//	//	}
	//	//	if (influence == 0)
	//	//	{
	//	//		GetWorldTimerManager().ClearTimer(spawningTimer);
	//	//		return;
	//	//	}
	//	//	GetWorldTimerManager().SetTimer(spawningTimer, this, &AVirusCalc::SpawningTimeElapsed, timerLength / influence, true);
	//	//}
	//}
}
