// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TankYouTriggerVolume.generated.h"

/**
 *
 */
UCLASS()
class TANKYOU_API ATankYouTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase* TriggerSound;
	UPROPERTY()
	class ATank* Tank;

private:

	UFUNCTION()
		virtual void OnBeginOverlap(AActor* OverlappingActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> TriggerHUD;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ATurret> BossBPClass;

	FVector BossSpawnLocation = FVector(1140.f, -1040.f, 9571.f);
	FRotator BossSpawnRotation = FRotator(0.f, 90.f, 0.f);

};
