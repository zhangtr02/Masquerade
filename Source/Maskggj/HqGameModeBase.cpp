// Fill out your copyright notice in the Description page of Project Settings.

#include "HqGameModeBase.h"
#include "MaskPlayerController.h"
#include "HqGameInstanceSubsystem.h"

void AHqGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PC = Cast<AMaskPlayerController>(GetWorld()->GetPlayerControllerIterator()->Get());
    GameI = GetGameInstance()->GetSubsystem<UHqGameInstanceSubsystem>();
}

void AHqGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    PC = Cast<AMaskPlayerController>(NewPlayer);
    if (PC)
    {
        PC->OnStatsChanged.RemoveDynamic(this, &AHqGameModeBase::OnStatsChangedHandler);
        PC->OnStatsChanged.AddDynamic(this, &AHqGameModeBase::OnStatsChangedHandler);
    }
}

void AHqGameModeBase::StartMainUI()
{
    if (PC)
    {
        PC->InitialDialogueWidget();
    }
}

void AHqGameModeBase::OnStatsChangedHandler(int32 Intel, int32 Charm, int32 Stamina, int32 Index)
{
    if (!PC || !GameI) return;

    FName EndingRowName = NAME_None;
    
    if (!GameI->RandomRowNames.IsValidIndex(Index))
    {
        EndingRowName = TEXT("Ending_TE");
    }
    
    if (Intel == 0) EndingRowName = TEXT("Ending_LowIntel");
    else if (Charm == 0) EndingRowName = TEXT("Ending_LowCharm");
    else if (Stamina == 0) EndingRowName = TEXT("Ending_LowStamina");
    
    if (Intel == 100) EndingRowName = TEXT("Ending_HighIntel");
    else if (Charm == 100) EndingRowName = TEXT("Ending_HighCharm");
    else if (Stamina == 100) EndingRowName = TEXT("Ending_HighStamina");

    if (!EndingRowName.IsNone())
    {
        PC->GameOver();
        
        PC->ShowEndingUI(EndingRowName);
    }
}
