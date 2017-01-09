// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Navigation/PathFollowingComponent.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "AI/Navigation/NavigationData.h"
#include "LaserSentryCustomPFC.generated.h"

/**
 * 
 */
UCLASS()
class TANKTRYCPP_API ULaserSentryCustomPFC : public UPathFollowingComponent
{
	GENERATED_BODY()

		/** follow current path segment */
		virtual void FollowPathSegment(float DeltaTime) override;

	/** sets variables related to current move segment */
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	/** check state of path following, update move segment if needed */
	virtual void UpdatePathSegment() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	const ANavigationData* GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound) const;
	//Choose Which Nav Data To Use
	const ANavigationData* ChooseNavData() const;
	//VERY IMPORTANT FOR CRASH PROTECTION !!!!!
	bool TileIsValid(const ARecastNavMesh* NavMesh, int32 TileIndex) const;

	//add this to your custom path follow component!
	bool NavPoly_GetAllPolys(TArray<NavNodeRef>& Polys);
};