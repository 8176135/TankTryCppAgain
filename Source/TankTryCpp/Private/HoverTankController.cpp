// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "HoverTankController.h"

AHoverTankController::AHoverTankController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AHoverTankController::BeginPlay()
{
	FActorSpawnParameters spawnParms = FActorSpawnParameters();
	spawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	transitionCamera = GetWorld()->SpawnActor<ASpecCamera>(specCamToSpawn, FVector(0, 0, 0), FRotator(0, 0, 0), spawnParms);
	transitionCamera->EEHandler->TransFinDele.BindUFunction(this, "MovementIsCompleted");
	ABaseTurret* startingTurret = GetWorld()->SpawnActor<ABaseTurret>(unitToSpawn, FVector(-272, 769, 116), FRotator(0, 0, 0), spawnParms);
	startingTurret->OnDestroyed.AddDynamic(this, &AHoverTankController::SubjectTurretIsDestroyed);
	allTurretsSpawned.AddUnique(startingTurret);
	Possess(startingTurret);
	tankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	hitDirections = TArray<FHitDir>();
	hitDirections.Reserve(5);
	actualHUD = Cast<ABaseTurretHUD>(MyHUD);
	Super::BeginPlay();
}

void AHoverTankController::Tick(float DeltaTime)
{
	if (tankState)
	{
		if (IsValid(controlledPawn))
		{
			FHitResult hitRes;
			if (GetWorld()->LineTraceSingleByChannel(hitRes, controlledPawn->eyeCam->GetComponentLocation(),
				controlledPawn->eyeCam->GetComponentLocation() + controlledPawn->eyeCam->GetForwardVector() * 10000, ECollisionChannel::ECC_Visibility, FCollisionQueryParams(NAME_None, true, *allTurretsSpawned.GetData())))
			{
				tankState->AimLocation = hitRes.Location;
			}

			for (int i = 0; i < hitDirections.Num(); ++i)
			{
				hitDirections[i].lifetime -= DeltaTime;
				if (hitDirections[i].lifetime < 0)
				{
					hitDirections.RemoveAt(i);
					i--;
				}
			}
		}
		else
		{
			controlledPawn = nullptr;
		}
		for (int i = 0; i < allTurretsSpawned.Num(); ++i)
		{
			if (!IsValid(allTurretsSpawned[i]))
			{
				allTurretsSpawned.RemoveAt(i);
				i--;
				continue;
			}
			if (allTurretsSpawned[i] != controlledPawn && allTurretsSpawned[i] != possesionElict)
			{
				FRotator deltaRot = (allTurretsSpawned[i]->mainTurretSke->GetSocketLocation("Turret") - tankState->AimLocation).Rotation();
				FRotator correctRotation = FMath::RInterpTo(FRotator(allTurretsSpawned[i]->turretTruePitch, allTurretsSpawned[i]->turretYaw, 0), deltaRot, DeltaTime, 5);
				allTurretsSpawned[i]->turretYaw = correctRotation.Yaw;
				allTurretsSpawned[i]->turretTruePitch = correctRotation.Pitch;
			}
		}
	}
	Super::Tick(DeltaTime);
}

void AHoverTankController::SetPawn(APawn* inPawn)
{
	if (IsValid(inPawn))
	{
		if (inPawn->IsA(ABaseTurret::StaticClass()))
		{
			controlledPawn = Cast<ABaseTurret>(inPawn);
			controlledPawn->turretDmgDele.BindUFunction(this, "OnControlledTurretDamaged");
			controlledPawn->targetHitDele.BindUFunction(this, "OnControlledTurretHitEnemy");
		}
		else
		{
			controlledPawn = nullptr;
			allowJump = false;
		}
	}
	Super::SetPawn(inPawn);
}

void AHoverTankController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("LookUp", this, &AHoverTankController::PitchCamera);
	InputComponent->BindAxis("Turn", this, &AHoverTankController::YawCamera);

	InputComponent->BindAction("Respawn", IE_Pressed, this, &AHoverTankController::RequestRespawn);
	InputComponent->BindAction("JumpTurret", IE_Pressed, this, &AHoverTankController::JumpTurret);
	InputComponent->BindAction("BuildTurret", IE_Pressed, this, &AHoverTankController::BuildTurret);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AHoverTankController::BeginFiring);
	InputComponent->BindAction("Fire", IE_Released, this, &AHoverTankController::EndFiring);
	InputComponent->BindAction("Exit", IE_Released, this, &AHoverTankController::ExitGame);
}

void AHoverTankController::BeginFiring()
{
	if (allowJump)
	{
		for (ABaseTurret* turret : allTurretsSpawned)
		{
			if (IsValid(turret))
			{
				turret->BeginFiring();
			}
		}
	}
}

void AHoverTankController::EndFiring()
{
	if (allowJump)
	{
		for (ABaseTurret* turret : allTurretsSpawned)
		{
			if (IsValid(turret))
			{
				turret->StopFiring();
			}
		}
	}
}

void AHoverTankController::RequestRespawn()
{
	if (!IsValid(controlledPawn))
	{
		UCppFunctionList::PrintString("RespawnToBeImplemented");
	}
	else
	{
		UCppFunctionList::PrintString("Ur still alive");
	}
}

void AHoverTankController::JumpTurret()
{
	if (!allowJump || !IsValid(controlledPawn))
	{
		UCppFunctionList::PrintString("Still Jumping");
		return;
	}
	FVector startingLoc = controlledPawn->eyeCam->GetComponentLocation();
	FHitResult hitRes;
	FCollisionQueryParams colQ = FCollisionQueryParams();
	colQ.AddIgnoredActor(controlledPawn);
	bool hit = GetWorld()->LineTraceSingleByChannel(hitRes, startingLoc, controlledPawn->eyeCam->GetForwardVector() * 2000 + startingLoc, ECollisionChannel::ECC_Visibility, colQ);
	if (hit && hitRes.IsValidBlockingHit() && IsValid(hitRes.GetActor()) && hitRes.GetActor()->ActorHasTag("Turret"))
	{
		ABaseTurret* candidate = Cast<ABaseTurret>(hitRes.GetActor());
		if (candidate != NULL)
		{
			for (ABaseTurret* turret : allTurretsSpawned)
			{
				turret->StopFiring();
			}
			controlledPawn->turretDmgDele.Clear();
			controlledPawn->targetHitDele.Clear();
			possesionElict = candidate;
			possesionElict->invincibility = true;
			hitDirections.Empty(5);
			transitionCamera->SetActorLocationAndRotation(controlledPawn->eyeCam->GetComponentLocation(), controlledPawn->eyeCam->GetComponentRotation());
			controlledPawn = nullptr;
			Possess(transitionCamera);
			allowJump = false;
			transitionCamera->StartTransition(candidate->eyeCam->GetComponentLocation(), candidate->eyeCam->GetComponentRotation());
		}
		else
		{
			UCppFunctionList::PrintString("WHY ARE U STEALING MY TURRETS!!!");
		}
	}
	else
	{
		UCppFunctionList::PrintString("NANANANA");
	}
}

void AHoverTankController::BuildTurret()
{
	float turretCost = unitToSpawn.GetDefaultObject()->GetTurretCost();
	if (tankState->Scrap < turretCost)
	{
		UCppFunctionList::PrintString("Not enough Scrap");
		return;
	}
	turretCost -= unitToSpawn.GetDefaultObject()->GetTurretCost();
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseTurret* newTurret = GetWorld()->SpawnActor<ABaseTurret>(unitToSpawn, tankState->AimLocation, FRotator(0), spawnParams);
	if (IsValid(newTurret))
	{
		newTurret->OnDestroyed.AddDynamic(this, &AHoverTankController::SubjectTurretIsDestroyed);
		newTurret->ConfigureSpawn();
		allTurretsSpawned.AddUnique(newTurret);
	}
}

void AHoverTankController::ExitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}

void AHoverTankController::MovementIsCompleted()
{
	allowJump = true;
	if (IsValid(possesionElict))
	{
		Possess(possesionElict);
	}
	possesionElict->invincibility = false;
	possesionElict = nullptr;
}

void AHoverTankController::SubjectTurretIsDestroyed(AActor* DestroyedActor)
{
	allTurretsSpawned.Remove(Cast<ABaseTurret>(DestroyedActor));
	if (allowJump && (!IsValid(controlledPawn) || DestroyedActor->GetUniqueID() == controlledPawn->GetUniqueID()) && allTurretsSpawned.Num() > 0)
	{
		controlledPawn = nullptr;
		hitDirections.Empty(5);
		possesionElict = allTurretsSpawned[0];
		possesionElict->invincibility = true;
		Possess(transitionCamera);
		allowJump = false;
		transitionCamera->StartTransition(allTurretsSpawned[0]->eyeCam->GetComponentLocation(), allTurretsSpawned[0]->eyeCam->GetComponentRotation());
	}
}

void AHoverTankController::OnControlledTurretDamaged(FHitDir hitDamageInfo)
{
	hitDirections.Add(hitDamageInfo);
}

void AHoverTankController::OnControlledTurretHitEnemy(float DamageAmount)
{
	actualHUD->UpdateHitMarker();
}

void AHoverTankController::PitchCamera(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->turretTruePitch = FMath::ClampAngle(controlledPawn->turretTruePitch + AxisValue, -60.0f, 60.0f);
	}

}

void AHoverTankController::YawCamera(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->turretYaw = controlledPawn->turretYaw + AxisValue;
	}
}


