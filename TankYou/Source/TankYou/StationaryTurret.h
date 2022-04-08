// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "StationaryTurret.generated.h"

/**
 *
 */
UCLASS()
class TANKYOU_API AStationaryTurret : public ABasePawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:

	FTimerHandle FireRateTimerHandle;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
		float FireRate = 3.f;
	UPROPERTY(EditInstanceOnly, Category = "Combat")
		float FireDelay = -1.f;
};
