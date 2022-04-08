// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankYouTriggerVolume.h"
#include "TankYouTriggerVolume_CaptnLook.generated.h"

/**
 *
 */
UCLASS()
class TANKYOU_API ATankYouTriggerVolume_CaptnLook : public ATankYouTriggerVolume
{
	GENERATED_BODY()

private:

	virtual void OnBeginOverlap(AActor* OverlappingActor, AActor* OtherActor) override;

};
