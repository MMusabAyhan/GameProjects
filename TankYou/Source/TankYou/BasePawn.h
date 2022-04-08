// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TANKYOU_API ABasePawn : public APawn
{
	GENERATED_BODY()



public:
	// Sets default values for this pawn's properties
	ABasePawn();

	void RotateTurret(FVector LookAtTarget);
	virtual void Fire();
	void HandleDestruction();

	UPROPERTY(EditAnywhere, Category = "Combat")
		class USceneComponent* ProjectileSpawnPoint;

private:

	// Forward Declaration of Comps
	UPROPERTY(EditAnywhere,Category = "Combat")
		class UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, Category = "Combat")
		class UStaticMeshComponent* BaseMesh;
	UPROPERTY(EditAnywhere, Category = "Combat")
		class UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere,Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
		class USoundBase* DeathSound;
};
