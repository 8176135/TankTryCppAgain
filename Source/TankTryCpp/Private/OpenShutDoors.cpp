// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "OpenShutDoors.h"


// Sets default values for this component's properties
UOpenShutDoors::UOpenShutDoors()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenShutDoors::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UOpenShutDoors::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//
void UOpenShutDoors::OpenDoor(ADonNavigationManager* navManager, UPrimitiveComponent* doorPos, FName id)
{
//	FDonMeshIdentifier meshID = FDonMeshIdentifier(doorPos, id);
	//bool resultIsValid;
	//navManager->RemoveCollisionAtMesh(meshID, resultIsValid, id, true);
//	navManager->GetVoxelCollisionProfileFromMesh(meshID, resultIsValid, false, false, meshID.CustomCacheIdentifier, false, false, 1, true);
	//navManager->GetVoxelCollisionProfileFromMesh(meshID, resultIsValid, false, false, meshID.CustomCacheIdentifier, false, false, 1, true);

}
