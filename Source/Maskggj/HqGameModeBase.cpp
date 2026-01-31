// Fill out your copyright notice in the Description page of Project Settings.


#include "HqGameModeBase.h"
#include "TableItemList.h"
#include "EndStoryUserWidget.h"
#include "TitleUserWidget.h"
#include "MaskPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AHqGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    //游戏流程待加入主界面 目前是直接加载对话了
    PC = Cast<AMaskPlayerController>(GetWorld()->GetPlayerControllerIterator()->Get());
    PC->BGMComponent->SetSound(MainMusic);
    PC->BGMComponent->Play();

    UTitleUserWidget* TitleUI = CreateWidget<UTitleUserWidget>(PC, BeginWidgetClass);
    if (TitleUI)
    {
        TitleUI->AddToViewport(100); // 放在最前面
    }
    GameI = GetGameInstance()->GetSubsystem<UHqGameInstanceSubsystem>();

}

void AHqGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("[LRY] playercontroller postlogin"));
    PC = Cast<AMaskPlayerController>(NewPlayer);
    if (PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[LRY] add dynamic on PC"));
        PC->OnStatsChanged.AddDynamic(this, &AHqGameModeBase::OnStatsChangedHandler);
    }
}

void AHqGameModeBase::OnStatsChangedHandler(int32 Intel, int32 Charm, int32 Stamina)
{
    UE_LOG(LogTemp, Warning, TEXT("[LRY] Stats Changed - Intel: %d, Charm: %d, Stamina: %d"), Intel, Charm, Stamina);
    if (!PC) return;

    FName EndingRowName = NAME_None;

    if (Intel == 0) EndingRowName = TEXT("Ending_LowIntel");
    else if (Charm == 0) EndingRowName = TEXT("Ending_LowCharm");
    else if(Stamina == 0 )  EndingRowName = TEXT("Ending_LowStamina");
    if (Intel == 100) EndingRowName = TEXT("Ending_HighIntel");
    else if (Charm == 100) EndingRowName = TEXT("Ending_HighCharm");
    else if (Stamina == 100)  EndingRowName = TEXT("Ending_HighStamina");

    if (!EndingRowName.IsNone())
    {
        PC->GameOver();
        UE_LOG(LogTemp, Log, TEXT("[LRY] GameOver"));
        TriggerEnding(EndingRowName);
    }
}

void AHqGameModeBase::StartMainUI()
{
    PC->InitialDialogueWidget();
}

void AHqGameModeBase::TriggerEnding(FName EndingRowName)
{
    if (!PC || !EndingTable || !EndingWidgetClass) return;

    UE_LOG(LogTemp, Warning, TEXT("[LRY] Attempting to trigger ending: %s"), *EndingRowName.ToString());
    FEndItem* Row = EndingTable->FindRow<FEndItem>(EndingRowName, TEXT("GetEnding"));
    if (Row)
    {
        UEndStoryUserWidget* EndingUI = CreateWidget<UEndStoryUserWidget>(PC, EndingWidgetClass);
        if (EndingUI)
        {
            UTexture2D* EndImg = Row->Image.LoadSynchronous();;
            if (!EndImg) {
                UE_LOG(LogTemp, Warning, TEXT("[LRY] Warning: Ending Image for '%s' is NULL!"), *EndingRowName.ToString());
            }
            EndingUI->SetupWidget(EndImg, Row->EndStory);
            EndingUI->AddToViewport(100); // 放在最前面

            PC->CloseDialogueWidget();
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("[LRY] TriggerEnding Error: Cannot find row '%s' in EndingTable!"), *EndingRowName.ToString());
    }
}
