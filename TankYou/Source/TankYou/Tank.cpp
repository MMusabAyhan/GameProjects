// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankYouPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CollidingTankMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/TriggerVolume.h"
#include "Components/CapsuleComponent.h"
//#include "DrawDebugHelpers.h"

// Sets default values
ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	TankMovementComponent = CreateDefaultSubobject<UCollidingTankMovementComponent>(TEXT("Tank Movement Component"));
	TankMovementComponent->UpdatedComponent = RootComponent;

	Energy = MaxEnergy;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());

	MuzzleOverHeatValue = 0.f;
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult Hit;
		TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		//DrawDebugSphere(GetWorld(), Hit.Location, 20, 30, FColor::Red);


		RotateTurret(Hit.ImpactPoint);

		ReduceMuzzleHeat(DeltaTime);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	//Tank Specific Visual & Sound FXs

	if (DeathCameraShakeBPClass)
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(DeathCameraShakeBPClass);
	}

	bDead = true;
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAxis(TEXT("Boost"), this, &ATank::OnBoostDrainEnergy);
	PlayerInputComponent->BindAxis(TEXT("AdjustCameraAngle"), this, &ATank::AdjustCameraAngle);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABasePawn::Fire);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &ATank::StartBoost);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Released, this, &ATank::StopBoost);
}

void ATank::MoveForward(float Value)
{
	if (TankMovementComponent && (TankMovementComponent->UpdatedComponent == RootComponent))
	{

		FVector DeltaLocation(0);

		DeltaLocation = GetActorForwardVector() * Value * TankMovementComponent->MovementSpeed * GetWorld()->GetDeltaSeconds();

		TankMovementComponent->AddInputVector(DeltaLocation);

		if (Value == 0 && bMoving)
		{
			bMoving = false;
		}
		else if (Value != 0 && !bMoving)
		{
			bMoving = true;
		}

		if (Value == -1)
		{
			bMovingForward = false;
		}
		else
		{
			bMovingForward = true;
		}
	}

	// Tried to move the tank with AddMovementInput function as I did with characters but it did not work.
	// I guess this function works only with characters. It may also require some other things that I dont know to work.
	// Anyway, for now i will just assume this function is only valid with characters not valid with pawns!
	// 
	//AddMovementInput(GetActorForwardVector() * MovementSpeed * GetWorld()->GetDeltaSeconds(), Value);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation(0);

	DeltaRotation.Yaw = Value * TurnRate * GetWorld()->GetDeltaSeconds();

	if (bMoving)
	{
		if (bMovingForward)
		{
			AddActorLocalRotation(DeltaRotation, true);
		}

		else
		{
			AddActorLocalRotation(-1 * DeltaRotation, true);
		}
	}
	else
	{
		AddActorLocalRotation(DeltaRotation, true);
	}

}

void ATank::StartBoost()
{
	if (bCanBoost)
	{
		TankMovementComponent->MovementSpeed = TankMovementComponent->BoostedSpeed;
		bOutofEnergy = false;
	}
}

void ATank::StopBoost()
{
	TankMovementComponent->MovementSpeed = TankMovementComponent->BaseMovementSpeed;

	//bOutofEnergy = false;
}

void ATank::AdjustCameraAngle(float Value)
{
	FRotator currentCameraRotation = SpringArm->GetRelativeRotation();

	float cameraAngleChange =Value * CameraAngleChangeStep;

	FRotator newCameraRotation = currentCameraRotation + FRotator(cameraAngleChange, 0.f, 0.f);

	newCameraRotation.Pitch = FMath::Clamp(newCameraRotation.Pitch, CameraMinAngle, CameraMaxAngle);

	SpringArm->SetRelativeRotation(newCameraRotation);

	// SpringArm->SetRelativeRotation(SpringArm->GetRelativeRotation() + FRotator(Value * CameraAngleChangeStep, 0.f, 0.f));
}

void ATank::Fire()
{
	float canFireThreshold = (MuzzleOverHeatThreshold - SpamBlockTime);

	if (MuzzleOverHeatValue <= canFireThreshold)
	{
		ABasePawn::Fire();
		MuzzleOverHeatValue++;
		MuzzleOverHeatValue = FMath::Clamp(MuzzleOverHeatValue, 0.f, MuzzleOverHeatThreshold);
	};
}

void ATank::ReduceMuzzleHeat(float DeltaTime)
{
	if (MuzzleOverHeatValue != 0)
	{
		MuzzleOverHeatValue -= MuzzleCooldownRate * DeltaTime;
		MuzzleOverHeatValue = FMath::Clamp(MuzzleOverHeatValue, 0.f, MuzzleOverHeatThreshold);

		//MuzzleOverHeatValue--;
	}
}

float ATank::GetOverHeatPercentage() const
{

	return MuzzleOverHeatValue / MuzzleOverHeatThreshold;
}

void ATank::OnBoostDrainEnergy(float Value)
{
	float Deltasecs = GetWorld()->GetDeltaSeconds();

	// Boosting conditions:
	// -Shift Key Pressed and tank moving 
	// -Boost applicable
	// -Tank is not out of energy. -> This condition is added 
	//  so that player MUST repress the shift button to activate boost.

	if (Value == 1 && bMoving && bCanBoost && !bOutofEnergy)
	{
		Energy -= EnergyConsumeRate * Deltasecs;

		Energy = FMath::Clamp(Energy, 0.f, MaxEnergy);

		if (Energy == 0.f)
		{
			TankMovementComponent->MovementSpeed = TankMovementComponent->BaseMovementSpeed;
			bCanBoost = false;
			bOutofEnergy = true;

		}
	}

	//When it is not pressed
	else //if (Value == 0)
	{
		Energy += EnergyRechargeRate * Deltasecs;
		Energy = FMath::Clamp(Energy, 0.f, MaxEnergy);

		if (Energy >= CanBoostThreshold && !bCanBoost)
		{
			bCanBoost = true;
		}
	}
}

float ATank::GetEnergyPercentage() const
{

	return Energy / MaxEnergy;
}

void ATank::SetSpringArmLength(float Length)
{
	SpringArm->TargetArmLength = Length;
}

void ATank::SetSpringArmCollisionTest(bool bDoCollisionTest)
{
	SpringArm->bDoCollisionTest = bDoCollisionTest;
}

void ATank::SetSpringArmRotation(float CameraPitchAngle)
{
	SpringArm->SetRelativeRotation(FRotator(-CameraPitchAngle, 0.f, 0.f));
}
