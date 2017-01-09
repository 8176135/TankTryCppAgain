// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ActorComponent.h"
#include "EnemyEventHandler.generated.h"

UCLASS()
class TANKTRYCPP_API UEnemyEventHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UEnemyEventHandler();

	DECLARE_DELEGATE_OneParam(ChildIsBeingHurt, float);
	ChildIsBeingHurt HurtDele;

	DECLARE_DELEGATE(TransitionFinished);
	TransitionFinished TransFinDele;

};
