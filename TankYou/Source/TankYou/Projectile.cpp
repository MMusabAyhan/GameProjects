// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "StationaryTurret.h"
#include "Tank.h"
#include "TankYouGameMode.h"
#include "Turret.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Projectile Capsule"));
	RootComponent = Cast<USceneComponent>(ProjectileCapsule);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));	
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 1500.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
	TrailParticles->SetupAttachment(ProjectileMesh);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	TankYouGameMode = GetWorld()->GetAuthGameMode<ATankYouGameMode>();

	ProjectileCapsule->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	//&& !Cast<AStationaryTurret>(GetOwner())
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation(), GetActorRotation());
	}

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) 
	{
		// We destroy projectile here for the case that we die but there is a projectile we fired hit 
		// somewhere in game. Otherwise it would stay in world undeleted.

		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{	
		bool bHitAlly = Cast<ATurret>(GetOwner()) && Cast<ATurret>(OtherActor);

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, Hit.Location,GetActorRotation());
		}

		if (!TankYouGameMode->bGameOver && !bHitAlly)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		}

		if (HitSound && !Cast<AStationaryTurret>(GetOwner()))
		{
			UGameplayStatics::SpawnSoundAtLocation(this, HitSound, Hit.Location, GetActorRotation());
		}

		if (HitCameraShakeBPClass && !Cast<AStationaryTurret>(GetOwner()))
		{
			UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(HitCameraShakeBPClass);
		}
	}


	
	Destroy();
}





 