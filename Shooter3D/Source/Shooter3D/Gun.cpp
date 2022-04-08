// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	MaxRange = 1000;
	Damage = 50;
	MaxTotalAmmo = 180;
	TotalAmmo = 120;
	MaxClipAmmo = 30;
	ClipAmmo = 30;
	ReloadTime = 2;
	isReloading = false;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	if (ClipAmmo == 0)
	{
		if (!isReloading)
		{
			isReloading = true;

			//Play Clip Empty Sound		
			GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AGun::Reload, ReloadTime, false);		
		}

		return;
	}

	ClipAmmo -= 1;

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitSound,Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

void AGun::Reload()
{
	// Missing ammo = the ammo number that will be refilled back into the magazine.
	int missingAmmo = MaxClipAmmo - ClipAmmo;

	if (missingAmmo < TotalAmmo)
	{
		ClipAmmo = MaxClipAmmo;
		TotalAmmo -= missingAmmo;
	}

	else
	{
		ClipAmmo += TotalAmmo;
		TotalAmmo = 0;
	}

	isReloading = false;
}

bool AGun::GunTrace(FHitResult& Hit, FVector& OutShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}
	FVector Location;
	FRotator Rotation;	
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	OutShotDirection = -Rotation.Vector(); 

	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End,ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}

