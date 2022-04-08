// Fill out your copyright notice in the Description page of Project Settings.


#include "TankYouTriggerVolume_CaptnLook.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

void ATankYouTriggerVolume_CaptnLook::OnBeginOverlap(AActor* OverlappingActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		if (Tank == Cast<ATank>(OtherActor) && TriggerSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, TriggerSound, GetActorLocation());
		}
	}
}