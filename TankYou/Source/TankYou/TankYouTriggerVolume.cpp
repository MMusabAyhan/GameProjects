// Fill out your copyright notice in the Description page of Project Settings.


#include "TankYouTriggerVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Turret.h"
#include "Blueprint/UserWidget.h"


void ATankYouTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATankYouTriggerVolume::OnBeginOverlap);

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

}

void ATankYouTriggerVolume::OnBeginOverlap(AActor* OverlappingActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		if (Tank == Cast<ATank>(OtherActor) && TriggerSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, TriggerSound, GetActorLocation());

			Tank->SetSpringArmLength(1600.f);
			Tank->SetSpringArmCollisionTest(false);

			UUserWidget* BossStageHUDWidget = CreateWidget(UGameplayStatics::GetPlayerController(this, 0), TriggerHUD);

			if (BossStageHUDWidget)
			{
				BossStageHUDWidget->AddToViewport();
			}

			//Spawning Boss

			if (BossBPClass)
			{
				ATurret* myBoss = nullptr;

				myBoss = GetWorld()->SpawnActor<ATurret>(BossBPClass, BossSpawnLocation, BossSpawnRotation);

				// If Boss created w/o an error
				if (myBoss)
				{
					myBoss->Tags.Add(TEXT("Boss"));
				}				
			}
		}
	}
}