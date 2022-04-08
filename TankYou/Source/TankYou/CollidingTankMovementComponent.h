// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CollidingTankMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TANKYOU_API UCollidingTankMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	UCollidingTankMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float MovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float BaseMovementSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float BoostedSpeed = 1000.f;

};
