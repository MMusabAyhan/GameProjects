// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Projectile.h"
//#include "DrawDebugHelpers.h"


// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Radius Check to Rotate Turret - Ex. for Radius Trigger

	if (isInFireRange() && isInSight())
	{
		// If so, rotate turret towards Tank

		RotateTurret(Tank->GetActorLocation());
	}
}

void ATurret::HandleDestruction() 
{
	Super::HandleDestruction();

	Destroy();

	// Turret Specific Visual & Sound FXs
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireConditionWithTimer, FireRate, true);
}

void ATurret::CheckFireConditionWithTimer()
{
	if (isInFireRange() && isInSight() && Tank && !Tank->bDead)
	{
		// If so, Fire!

		Fire();
	}
}

//Checks if tank is in turret's fire range.
bool ATurret::isInFireRange()
{
	if (Tank)
	{
		float distfromTank = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());

		return distfromTank <= FireRange;
	}

	return false;
}


// This Check is added to fix the issue with turrets firing the tank behind walls or other objects.
// Here, it  is is checked whether there is any object between tank and turret that blocks the vision
// of turret.
bool ATurret::isInSight()
{
	if (Tank)
	{
		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = Tank->GetActorLocation();

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1);

		FCollisionQueryParams Params;

		TArray<AActor*> TurretArray;
		UGameplayStatics::GetAllActorsOfClass(this, ATurret::StaticClass(), TurretArray);

		TArray<AActor*> SpawnedProjectileArray;
		UGameplayStatics::GetAllActorsOfClass(this, AProjectile::StaticClass(), SpawnedProjectileArray);

		// All other turrets, projectiles that exist in world and tank itself is added to the
		// ignored actors list.
		Params.AddIgnoredActor(Tank);
		Params.AddIgnoredActors(TurretArray);
		Params.AddIgnoredActors(SpawnedProjectileArray);

		return  !GetWorld()->LineTraceTestByChannel(Start, End, ECC_GameTraceChannel1, Params);
	}

	return false;
}