// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUps.generated.h"

UCLASS()
class TANKYOU_API APickUps : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUps();

	UFUNCTION(BlueprintImplementableEvent)
		void OnPickUp(class UPrimitiveComponent* OverlappingComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ItemMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
