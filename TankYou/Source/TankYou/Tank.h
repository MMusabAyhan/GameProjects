// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

UCLASS()
class TANKYOU_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();
	void HandleDestruction();
	APlayerController* GetTankPlayerController() const { return TankPlayerController; }
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Status")
		bool bDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float Energy;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float MaxEnergy = 100.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float EnergyRechargeRate = 40.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float EnergyConsumeRate = 30.f;
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Movement")
		float CanBoostThreshold = 25.f;

	bool bCanBoost = true;
	bool bOutofEnergy = false;

	void Fire();
	void SetSpringArmLength(float Length);
	void SetSpringArmCollisionTest(bool bDoCollisionTest);
	UFUNCTION(BlueprintCallable)
		void SetSpringArmRotation(float CameraPitchAngle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Specific Components
	// Camera

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<class UCameraShakeBase> DeathCameraShakeBPClass;

	class APlayerController* TankPlayerController;

	// Movement
	// Tank Movement Comp for SlideAlongSurface feature

	class UCollidingTankMovementComponent* TankMovementComponent;

	// Components for Setting Muzzle OverHeat Feature

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float MuzzleOverHeatThreshold = 5.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float MuzzleOverHeatValue;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		float SpamBlockTime = 0.5f;

	UPROPERTY(EditInstanceOnly, Category = "Combat")
		float MuzzleCooldownRate = 1.5f;

	//Functions
	//Input Functions

	void MoveForward(float Value);
	void Turn(float Value);
	void AdjustCameraAngle(float Value);
	void StartBoost();
	void StopBoost();
	void OnBoostDrainEnergy(float Value);

	//OtherFunctions

	void ReduceMuzzleHeat(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		float GetOverHeatPercentage() const;
	UFUNCTION(BlueprintCallable)
		float GetEnergyPercentage() const;

	//Variables

	bool bMovingBackwardFirst = false;
	bool bTurning = false;
	bool bMoving = false;
	bool bMovingForward = true;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float TurnRate = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		float CameraAngle = 45.f;

	float CameraMinAngle = -70.f;
	float CameraMaxAngle = -25.f;
	float CameraAngleChangeStep = 5.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
