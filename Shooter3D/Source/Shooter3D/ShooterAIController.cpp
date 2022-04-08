// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"
#include <Shooter3D/ShooterCharacter.h>

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();   
    
    if (AIBehavior != nullptr) 
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
        RunBehaviorTree(AIBehavior); 
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),GetPawn()->GetActorLocation());
       
        
    }
}
void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}
