// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "LaserSentryCustomPFC.h"
#include "CppFunctionList.h"


void ULaserSentryCustomPFC::FollowPathSegment(float DeltaTime)
{
	//UCppFunctionList::PrintString(TEXT("FollowPathSeg"));

	Super::FollowPathSegment(DeltaTime);
}

void ULaserSentryCustomPFC::SetMoveSegment(int32 SegmentStartIndex)
{
	//UCppFunctionList::PrintString(TEXT("SetMoveSeg"));
	Super::SetMoveSegment(SegmentStartIndex);
}

void ULaserSentryCustomPFC::UpdatePathSegment()
{
	//UCppFunctionList::PrintString(TEXT("UpdatePath"));
	Super::UpdatePathSegment();
}

void ULaserSentryCustomPFC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//UCppFunctionList::PrintVector(CurrentDestination.Position);
	//UE_LOG(LogTemp, Warning, TEXT("TICK---4"));
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

const ANavigationData* ULaserSentryCustomPFC::GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound) const
{
	UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
	if (!NavSys) return NULL;
	return NavSys->GetMainNavData(CreateNewIfNoneFound);
}

const ANavigationData* ULaserSentryCustomPFC::ChooseNavData() const
{
	const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
	const ANavigationData* NavData = GetWorld()->GetNavigationSystem()->GetNavDataForProps(AgentProperties);
	if (NavData == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("ERROR USING MAIN NAV DATA"));
		NavData = this->GetMainNavData(FNavigationSystem::ECreateIfEmpty());
	}

	return NavData;
}

bool ULaserSentryCustomPFC::TileIsValid(const ARecastNavMesh* NavMesh, int32 TileIndex) const
{
	if (!NavMesh) return false;
	//~~~~~~~~~~~~~~
	const FBox TileBounds = NavMesh->GetNavMeshTileBounds(TileIndex);

	return TileBounds.IsValid != 0;
}

bool ULaserSentryCustomPFC::NavPoly_GetAllPolys(TArray<NavNodeRef>& Polys)
{
	if (!MovementComp) return false;
	//~~~~~~~~~~~~~\\
		
//Get Nav Data
	const ANavigationData* NavData = ChooseNavData();

	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
	if (!NavMesh)
	{
		return false;
	}

	TArray<FNavPoly> EachPolys;
	for (int32 v = 0; v < NavMesh->GetNavMeshTilesCount(); v++)
	{

		//CHECK IS VALID FIRST OR WILL CRASH!!! 
		//     256 entries but only few are valid!
		// use continue in case the valid polys are not stored sequentially
		if (!TileIsValid(NavMesh, v))
		{
			continue;
		}

		NavMesh->GetPolysInTile(v, EachPolys);
	}


	//Add them all!
	for (int32 v = 0; v < EachPolys.Num(); v++)
	{
		Polys.Add(EachPolys[v].Ref);
	}
	return true;
}

