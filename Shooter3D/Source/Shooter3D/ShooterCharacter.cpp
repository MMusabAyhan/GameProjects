// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "Shooter3DGameModeBase.h"
#include <Shooter3D/ShooterPlayerController.h>
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActiveGun = nullptr;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnSpawnArrangeWeapons();

	Health = MaxHealth;

	if (Cast<AShooterPlayerController>(GetController()))
	{
		this->GetCharacterMovement()->AirControl = 0.66;
		this->GetCharacterMovement()->AirControlBoostMultiplier = 2.0;
		this->GetCharacterMovement()->FallingLateralFriction = 0.22;
		this->GetCharacterMovement()->JumpZVelocity = 400.0;
	}

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("ControllerLookUp"), this, &AShooterCharacter::ControllerLookUp);
	PlayerInputComponent->BindAxis(TEXT("ControllerLookRight"), this, &AShooterCharacter::ControllerLookRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("SwitchToRifle"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchToRifle);
	PlayerInputComponent->BindAction(TEXT("SwitchToLauncher"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchToLauncher);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
void AShooterCharacter::ControllerLookUp(float AxisValue)
{
	AddControllerPitchInput(RotationRate * AxisValue * GetWorld()->GetDeltaSeconds());
}
void AShooterCharacter::ControllerLookRight(float AxisValue)
{
	AddControllerYawInput(RotationRate * AxisValue * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	if (ActiveGun)
	{
		ActiveGun->PullTrigger();
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(DamageToApply, Health);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Damn! I am Hit. I got %f Health left"), Health);

	if (IsDead())
	{
		AShooter3DGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooter3DGameModeBase>();

		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercentage() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::SwitchToRifle()
{
	ActiveIndex = 0;
	ActiveGun = GunArray[ActiveIndex];

	ActiveGun->isReloading = false;
	GetWorldTimerManager().ClearTimer(ActiveGun->ReloadTimerHandle);

	for (int Index = 0; Index < GunArray.Num(); Index++)
	{
		if (Index == ActiveIndex)
		{
			GunArray[Index]->SetActorHiddenInGame(false);
		}
		else
		{
			GunArray[Index]->SetActorHiddenInGame(true);
		}
	}
}

void AShooterCharacter::SwitchToLauncher()
{
	ActiveIndex = 1;
	ActiveGun = GunArray[ActiveIndex];

	ActiveGun->isReloading = false;
	GetWorldTimerManager().ClearTimer(ActiveGun->ReloadTimerHandle);

	for (int Index = 0; Index < GunArray.Num(); Index++)
	{
		if (Index == ActiveIndex)
		{
			GunArray[Index]->SetActorHiddenInGame(false);
		}
		else
		{
			GunArray[Index]->SetActorHiddenInGame(true);
		}
	}
}

// This Function spawns all weapons and deal with their arrangements such as storing them in the gun array, hiding them and attaching them to the relevant socket.
// It also sets the current active gun. 
void AShooterCharacter::OnSpawnArrangeWeapons()
{
	//Spawning and storing in array
	Rifle = GetWorld()->SpawnActor<AGun>(RifleClass);
	Launcher = GetWorld()->SpawnActor<AGun>(LauncherClass);

	GunArray.Emplace(Rifle);
	GunArray.Emplace(Launcher);

	//Hiding and attachments
	for (AGun* Gun : GunArray)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		Gun->SetActorHiddenInGame(true);
	}

	//Setting Active Gun
	ActiveGun = GunArray[0];
	ActiveGun->SetActorHiddenInGame(false);

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
}

void AShooterCharacter::Reload()
{
	if (ActiveGun)
	{
		GetWorldTimerManager().SetTimer(ActiveGun->ReloadTimerHandle, ActiveGun, &AGun::Reload, ActiveGun->ReloadTime, false);
	}
}