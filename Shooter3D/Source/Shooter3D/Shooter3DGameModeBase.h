// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Shooter3DGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER3D_API AShooter3DGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PawnKilled(APawn* PawnKilled);
	
};
