// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DonAINavigation/Classes/DonNavigationManager.h"
#include "OpenShutDoors.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKTRYCPP_API UOpenShutDoors : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenShutDoors();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "AI Navigation")
		void OpenDoor(ADonNavigationManager* navManager, UPrimitiveComponent* doorPos, FName id);
};
