// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "DrawDebugHelpers.h"
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
	transitionCamera->TransFinDele.BindUFunction(this, "MovementIsCompleted");
	turretHologram = GetWorld()->SpawnActor<ATurretPlaceholderBase>(placeholderTurret, FVector(0, 0, 0), FRotator(0, 0, 0), spawnParms);
	turretHologram->SetVisibility(false);
	ABaseTurret* startingTurret = GetWorld()->SpawnActor<ABaseTurret>(unitToSpawn, FVector(-272, 769, 116), FRotator(0, 0, 0), spawnParms);
	startingTurret->OnDestroyed.AddDynamic(this, &AHoverTankController::SubjectTurretIsDestroyed);
	startingTurret->allTurrets = &allTurretsSpawned;
	allTurretsSpawned.AddUnique(startingTurret);
	Possess(startingTurret);
	tankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	tankState->playerController = this;
	hitDirections = TArray<FHitDir>();
	hitDirections.Reserve(5);
	actualHUD = Cast<ABaseTurretHUD>(MyHUD);

	Super::BeginPlay();
}

void AHoverTankController::Tick(float DeltaTime)
{
	if (!tankState)
	{
		return;
	}

	if (IsValid(controlledPawn))
	{
		FHitResult hitRes;
		if (GetWorld()->LineTraceSingleByObjectType(hitRes, controlledPawn->eyeCam->GetComponentLocation(),
			controlledPawn->eyeCam->GetComponentLocation() + controlledPawn->eyeCam->GetForwardVector() * 10000, eyeBlockingObjects, FCollisionQueryParams(NAME_None, true, *allTurretsSpawned.GetData())))
		{
			tankState->AimLocation = hitRes.Location;
		}
		tankState->inaccuracy = UCppFunctionList::CosineGraph(-controlledPawn->maxInaccuracy, 1, controlledPawn->GetSpreadVal());
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

	if (prepToBuild)
	{
		turretHologram->SetActorLocation(tankState->AimLocation);
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
			controlledPawn->eyeCam->FieldOfView = defaultPOV;
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
	InputComponent->BindAction("BuildTurret", IE_Pressed, this, &AHoverTankController::RequestTurretBuild);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AHoverTankController::BeginFiring);
	InputComponent->BindAction("Fire", IE_Released, this, &AHoverTankController::EndFiring);
	InputComponent->BindAction("Zoom", IE_Pressed, this, &AHoverTankController::ZoomIn);
	InputComponent->BindAction("Zoom", IE_Released, this, &AHoverTankController::ZoomOut);
	InputComponent->BindAction("ShowHealth", IE_Pressed, this, &AHoverTankController::ShowHpStats);
	InputComponent->BindAction("ShowHealth", IE_Released, this, &AHoverTankController::HideHpStats);
	InputComponent->BindAction("Exit", IE_Released, this, &AHoverTankController::ExitGame);
}

void AHoverTankController::BeginFiring()
{
	if (allowJump)
	{
		if (prepToBuild)
		{
			if (turretHologram->ableToSpawn && BuildTurret(tankState->AimLocation))
			{
				prepToBuild = false;
				turretHologram->SetVisibility(false);
			}
		}
		else
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
}

void AHoverTankController::EndFiring()
{
	for (ABaseTurret* turret : allTurretsSpawned)
	{
		if (IsValid(turret))
		{
			turret->StopFiring();
		}
	}
}

void AHoverTankController::ZoomIn()
{
	if (allowJump && IsValid(controlledPawn))
	{
		if (prepToBuild)
		{
			prepToBuild = false;
			turretHologram->SetVisibility(false);
		}
		else
		{
			controlledPawn->eyeCam->FieldOfView = defaultPOV / 1.5;
		}
	}
}

void AHoverTankController::ZoomOut()
{
	controlledPawn->eyeCam->FieldOfView = defaultPOV;
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
			HideHpStats();
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

void AHoverTankController::RequestTurretBuild()
{
	prepToBuild = true;
	turretHologram->SetActorLocation(tankState->AimLocation, false);
	turretHologram->SetVisibility(true);
}

void AHoverTankController::ShowHpStats()
{
	if (!allowJump || !IsValid(controlledPawn))
	{
		return;
	}
	for (ABaseTurret* turret : allTurretsSpawned)
	{
		if (turret->GetUniqueID() != controlledPawn->GetUniqueID())
		{
			turret->SetWidgetVisibility(true);
		}
	}
}

void AHoverTankController::HideHpStats()
{
	for (ABaseTurret* turret : allTurretsSpawned)
	{
		turret->SetWidgetVisibility(false);
	}
}

bool AHoverTankController::BuildTurret(FVector buildLocation)
{
	float turretCost = unitToSpawn.GetDefaultObject()->GetTurretCost();
	if (tankState->Scrap < turretCost)
	{
		UCppFunctionList::PrintString("Not enough Scrap");
		return false;
	}
	tankState->Scrap -= turretCost;
	
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseTurret* newTurret = GetWorld()->SpawnActor<ABaseTurret>(unitToSpawn, buildLocation, FRotator(0), spawnParams);
	if (IsValid(newTurret))
	{
		newTurret->OnDestroyed.AddDynamic(this, &AHoverTankController::SubjectTurretIsDestroyed);
		newTurret->ConfigureSpawn();
		newTurret->allTurrets = &allTurretsSpawned;
		allTurretsSpawned.AddUnique(newTurret);
		return true;
	}
	else
	{
		return false;
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


