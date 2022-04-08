// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SHOOTER3D_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	AShooterCharacter();
	void Shoot();

	void Reload();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGun* ActiveGun;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<AGun*> GunArray;
private:

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ControllerLookUp(float AxisValue);
	void ControllerLookRight(float AxisValue);
	void SwitchToRifle();
	void SwitchToLauncher();

	UPROPERTY(EditAnywhere)
	float RotationRate = 60;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> RifleClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> LauncherClass;

	UPROPERTY()
	AGun* Rifle;
	
	UPROPERTY()
	AGun* Launcher;

	UPROPERTY()
	int ActiveIndex{ 0 };

	UPROPERTY(VisibleAnywhere)
	float Health;

	void OnSpawnArrangeWeapons();

};
