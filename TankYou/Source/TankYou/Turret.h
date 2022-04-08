// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Turret.generated.h"

/**
 * 
 */
UCLASS()
class TANKYOU_API ATurret : public ABasePawn
{
	GENERATED_BODY()
public:

	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ATank* Tank;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.f;
	void CheckFireConditionWithTimer();
	bool isInFireRange();
	bool isInSight();

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 200.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
