// Fill out your copyright notice in the Description page of Project Settings.


#include "StationaryTurret.h"

void AStationaryTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABasePawn::Fire, FireRate, true,FireDelay);


}

