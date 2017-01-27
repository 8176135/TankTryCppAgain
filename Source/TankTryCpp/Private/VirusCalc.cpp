// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "TankStateCpp.h"
#include "CppFunctionList.h"
#include "VirusCalc.h"
#include <EnemyBaseClass.h>

//#include "Virus"


// Sets default values
AVirusCalc::AVirusCalc()
{
	//VariableSetup
	PrimaryActorTick.bCanEverTick = true;
	FAttachmentTransformRules tempATR(EAttachmentRule::KeepRelative, false);
	//vOQP = FCollisionObjectQueryParams();
	//for (auto collisionChannel : ObstacleQueryChannels)
	//	vOQP.AddObjectTypesToQuery(collisionChannel);

	//ComponentSetup
	sceneComp = CreateDefaultSubobject<USceneComponent>("TheRootOfAllEvil");
	billboardMarker = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard Marker"));
	SetRootComponent(sceneComp);
	billboardMarker->AttachToComponent(RootComponent, tempATR);

//	ppShapes = CreateDefaultSubobject<UBoxComponent>("ShapeC");
	//ppShapes->AttachToComponent(sceneComp, tempATR);
	
	//instancedVirus->SetStaticMesh(boxMesh);
	instancedVirus = CreateDefaultSubobject<UInstancedStaticMeshComponent>("VirusItself");
	instancedVirus->AttachToComponent(sceneComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	instancedVirus->InstanceEndCullDistance = 10000;
	instancedVirus->InstanceStartCullDistance = 9500;

	Tags.Add("Virus");

	unifiedPPSettings.bOverride_WhiteTint = 1;
	unifiedPPSettings.bOverride_LensFlareIntensity = 1;
	unifiedPPSettings.bOverride_BloomIntensity = 1;

	allVirusData.Reserve(10000);
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

	instancedVirus->SetCollisionProfileName("VirusCol");
	//ppShapes->AddShapeToGeomArray<UBoxComponent>();
	Super::BeginPlay();
}

// Called every frame
void AVirusCalc::Tick(float DeltaTime)
{
	storage.UpdateValues(DeltaTime);
	unifiedPPSettings.WhiteTint = FMath::Pow(FMath::Cos(FMath::Pow(FMath::Sin(storage.WhiteTint), 2)), 7) * 2 - 1;
	unifiedPPSettings.LensFlareIntensity = -FMath::Cos(storage.LensFlareIntensity * 2) / 2 + 1;
	unifiedPPSettings.BloomIntensity = FMath::Cos(storage.BloomIntensity) * 3 + 4;

	for (TPair<FVector, FVirusPart> data : allVirusData)
	{
		if (data.Value.dead)
		{
			continue;
		}
		//data.Value.PPComponent->Settings = unifiedPPSettings;
		if (!data.Value.full)
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
			//influencers.Empty();
			FTransform currentVirTrans;
			if (instancedVirus->GetInstanceTransform(data.Value.virusIndex, currentVirTrans))
			{
				FVector newScale = UCppFunctionList::Clamp3Numbers((DeltaTime * data.Value.numOfFullSizedNeighbours * FVector(spawnSpeed) * (1 - influencerPower) + currentVirTrans.GetScale3D()), 1, 1, 1, 0, 0, 0);
				if (spawnSpeed != 0)
				{
					currentVirTrans.SetScale3D(newScale);
					instancedVirus->UpdateInstanceTransform(data.Value.virusIndex, currentVirTrans);
				}
				if (newScale == FVector(1))
				{
					allVirusData[data.Key].full = true;
					if (!data.Value.blocked)
					{
						CheckForNeighbours(data.Key, true);
						QueueVirusesToSpawn(data.Key);
					}
				}
			}
			else
			{
				UCppFunctionList::PrintString("Well ur indexing thing has a problem --> %f", data.Value.virusIndex);
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
					//instancedVirus->UpdateInstanceTransform()
						//data.Value.meshComponent->SetMaterial(0, fadingMaterial);
				}
				//data.Value.PPComponent->BlendWeight = data.Value.lifetime * 0.7 / data.Value.fadingTime;
				FTransform currentTrans;
				if (instancedVirus->GetInstanceTransform(data.Value.virusIndex, currentTrans))
				{
					FVector newScale = FMath::Lerp(FVector(0, 0, 0), FVector(1, 1, 1), data.Value.lifetime / data.Value.fadingTime);
					currentTrans.SetScale3D(newScale);
					instancedVirus->UpdateInstanceTransform(data.Value.virusIndex, currentTrans);
				}
				else
				{
					UCppFunctionList::PrintString("Well ur indexing thing has a problem 2");
				}
			}
		}
	}
	instancedVirus->MarkRenderStateDirty();
	TArray<AActor*> allOverlappers = TArray<AActor*>();
	instancedVirus->GetOverlappingActors(allOverlappers);
	if (allOverlappers.Num() > 0)
	{
		for (AActor* actor : allOverlappers)
		{
			if (actor->ActorHasTag("Enemy"))
			{
				UGameplayStatics::ApplyDamage(actor, DeltaTime * dmgPerSecond, NULL, this, virusDmgTp);
			}
			else if (actor->ActorHasTag("Turret"))
			{
				UGameplayStatics::ApplyDamage(actor, DeltaTime * dmgPerSecond, NULL, this, virusDmgTp);
			}
		}
	}
	else
	{
		UCppFunctionList::PrintString("off");
	}

	Super::Tick(DeltaTime);
}

//void AVirusCalc::OverlapBegins(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (!IsValid(OtherActor))
//	{
//		UCppFunctionList::PrintString("Overlapped an invalid actor");
//		return;
//	}
//	if (OtherActor->ActorHasTag("Turret") || OtherActor->ActorHasTag("Enemy"))
//	{
//		UCppFunctionList::PrintString("ewfwef22we");
//		bool found = false;
//		for (int i = 0; i < allOverlapData.Num(); ++i)
//		{
//			if (!IsValid(allOverlapData[i].actor))
//			{
//				allOverlapData.RemoveAt(i);
//				i--;
//				continue;
//			}
//			if (allOverlapData[i].actor->GetUniqueID() == OtherActor->GetUniqueID())
//			{
//				allOverlapData[i].count++;
//				found = true;
//				break;
//			}
//		}
//		if (!found)
//		{
//			allOverlapData.Add(FOverlapStorage(OtherActor));
//		}
//	}
//}

//void AVirusCalc::OverlapEnds(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (!IsValid(OtherActor))
//	{
//		UCppFunctionList::PrintString("Overlapped an invalid actor");
//		return;
//	}
//	if (OtherActor->ActorHasTag("Turret") || OtherActor->ActorHasTag("Enemy"))
//	{
//		bool found = false;
//		for (int i = 0; i < allOverlapData.Num(); ++i)
//		{
//			if (!IsValid(allOverlapData[i].actor))
//			{
//				allOverlapData.RemoveAt(i);
//				i--;
//				continue;
//			}
//			if (allOverlapData[i].actor->GetUniqueID() == OtherActor->GetUniqueID())
//			{
//				allOverlapData[i].count--;
//				if (allOverlapData[i].count <= 0)
//				{
//					allOverlapData.RemoveAt(i);
//				}
//				found = true;
//				break;
//			}
//		}
//		if (!found)
//		{
//			//UCppFunctionList::PrintString(HitComp->GetName());
//			UCppFunctionList::PrintString(OtherActor->GetName());
//		}
//	}
//}

void AVirusCalc::SpawningVirus(FVector curLoc)
{
	FAttachmentTransformRules tempATR(EAttachmentRule::KeepRelative, false);
	//curloc * box = (y - actloc)/box
	FVector realLoc = FVector((curLoc.X * BoxSize) + GetActorLocation().X, (curLoc.Y * BoxSize) + GetActorLocation().Y, (curLoc.Z * BoxSize) + GetActorLocation().Z);
	TArray<FOverlapResult> overLapResults;
	GetWorld()->OverlapMultiByObjectType(overLapResults, realLoc, FQuat::Identity, ObstacleQueryChannels, vColShape, FCollisionQueryParams(NAME_None, false));

	int newVirusIndex = instancedVirus->AddInstanceWorldSpace(FTransform(FQuat::Identity, realLoc, FVector(0, 0, 0)));

	//UStaticMeshComponent* newStaticMeshComp = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Virus Part - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	//newStaticMeshComp->SetCastShadow(false);
	//newStaticMeshComp->AttachToComponent(RootComponent, tempATR);
	//newStaticMeshComp->SetRelativeScale3D(FVector(1, 1, 1));
	//newStaticMeshComp->SetWorldLocation(realLoc);
	//newStaticMeshComp->SetStaticMesh(boxMesh);
	//newStaticMeshComp->SetCollisionProfileName(FName("TemporaryPathBlock"));
	//newStaticMeshComp->RegisterComponent();

	//newStaticMeshComp->SetRelativeScale3D(FVector(0.01, 0.01, 0.01));
	//newStaticMeshComp->SetCollisionProfileName(FName("NoCollision"));

	//UBoxComponent* PPCVolume = NewObject<UBoxComponent>(this, FName(*FString::Printf(TEXT("BoxVol - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));

	//PPCVolume->SetBoxExtent(FVector(BoxSize / 2, BoxSize / 2, BoxSize / 2));
	//PPCVolume->AttachToComponent(RootComponent, tempATR);
	//PPCVolume->SetWorldLocation(realLoc);
	//PPCVolume->SetCollisionProfileName(FName("VirusCol"));
	////PPCVolume->OnComponentBeginOverlap.AddDynamic(this, &AVirusCalc::OverlapBegins);
	////PPCVolume->OnComponentEndOverlap.AddDynamic(this, &AVirusCalc::OverlapEnds);
	//PPCVolume->RegisterComponent();
	//bool validity;
	//navManager->ChangeCollisionAtMesh(PPCVolume, false, true, validity, NAME_None, false);
	//PPCVolume->UpdateOverlaps();

	//UPostProcessComponent* newPPC = NewObject<UPostProcessComponent>(this, FName(*FString::Printf(TEXT("PPC Sub - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	//newPPC->AttachToComponent(PPCVolume, tempATR);
	//newPPC->Priority = 2;
	//newPPC->BlendWeight = 0.7f;
	//newPPC->bUnbound = false;
	////newPPC->bEnabled = false;
	//newPPC->RegisterComponent();

	USphereComponent* EnemyInfluenceVol = NewObject<USphereComponent>(this, FName(*FString::Printf(TEXT("EIVol - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	EnemyInfluenceVol->SetSphereRadius(enemyInfluenceRadius);
	EnemyInfluenceVol->SetCollisionProfileName("EnemyFinder");
	EnemyInfluenceVol->AttachToComponent(RootComponent, tempATR);
	EnemyInfluenceVol->SetWorldLocation(realLoc);
	EnemyInfluenceVol->RegisterComponent();
	allVirusData.Add(curLoc, FVirusPart(realLoc, overLapResults.Num() != 0, newVirusIndex, EnemyInfluenceVol, totalLifetime, fadingTime));
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
	//bool validity;
	//navManager->ChangeCollisionAtMesh(allVirusData[deadLoc].PPVolume, true, true, validity, NAME_None, false);
	allVirusData[deadLoc].lifetime = 0;
	instancedVirus->RemoveInstance(allVirusData[deadLoc].virusIndex);
	//allVirusData[deadLoc].PPComponent->BlendWeight = 0.7f;
	//allVirusData[deadLoc].PPComponent->Settings = deadPPSettings;
	//allVirusData[deadLoc].PPVolume->bGenerateOverlapEvents = false;
	allVirusData[deadLoc].dead = true;

	for (TPair<FVector, FVirusPart> data : allVirusData)
	{
		if (data.Value.dead)
			continue;
		allVirusData[data.Key].UpdateVirusIndex(allVirusData[deadLoc].virusIndex);
	}
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
				&& !allVirusData[checkingLocation].blocked && allVirusData[checkingLocation].full)
			{
				allVirusData[blockToCheck].numOfFullSizedNeighbours++;
			}
		}
	}
	else if (allVirusData[blockToCheck].full)
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
		//UCppFunctionList::PrintVector(instancedVirus->InstanceBodies[allVirusData[blockToCheck].virusIndex]->Scale3D);
		UCppFunctionList::PrintString("Passed a block that is not full for an inverse check");
		return;
	}
}

//void AVirusCalc::ForceUpdate()
//{
//	for (auto data : allVirusData)
//	{
//		if (!data.Value.dead)
//		{
//			if (data.Value.meshComponent->RelativeScale3D != FVector(1))
//			{
//				allVirusData[data.Key].numOfFullSizedNeighbours = 0;
//				for (FVector direction : allDirections)
//				{
//					FVector checkingLocation = data.Key + direction;
//					if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead
//						&& !allVirusData[checkingLocation].blocked && allVirusData[checkingLocation].meshComponent->RelativeScale3D == FVector(1))
//					{
//						allVirusData[data.Key].numOfFullSizedNeighbours++;
//					}
//				}
//			}
//			else if (!data.Value.blocked)
//			{
//				QueueVirusesToSpawn(data.Key);
//			}
//		}
//	}
//}

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
