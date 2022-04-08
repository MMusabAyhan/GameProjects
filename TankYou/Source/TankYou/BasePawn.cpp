// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/World.h"




// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creating Components and Setting up necessary attachments

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = Cast<USceneComponent>(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);


}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector LookDirection = LookAtTarget - TurretMesh->GetComponentLocation();
	TurretMesh->SetWorldRotation(FRotator(0, LookDirection.Rotation().Yaw, 0));

}

void ABasePawn::Fire()
{
	if (ProjectileClass)
	{
		FActorSpawnParameters mySpawnParameters;
		mySpawnParameters.Owner = this;

		//Spawning the projectile
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation,mySpawnParameters);
		//Projectile->SetOwner(this);
	}

}

void ABasePawn::HandleDestruction()
{
	// Visual&Sound FXs
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		// You can also use PlaySoundAtLocation. They are same as long as you keep bAutoDestroy as true for SpawnSoundAtLocation :)
		UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}
}