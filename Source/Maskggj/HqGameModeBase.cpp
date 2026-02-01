// Fill out your copyright notice in the Description page of Project Settings.


#include "HqGameModeBase.h"
#include "TableItemList.h"
#include "EndStoryUserWidget.h"
#include "MaskPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AHqGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PC = Cast<AMaskPlayerController>(GetWorld()->GetPlayerControllerIterator()->Get());

    PC->SetShowMouseCursor(true);
    FInputModeGameAndUI Mode;
    Mode.SetHideCursorDuringCapture(false);
    PC->SetInputMode(Mode);

    TitleUI = CreateWidget<UUserWidget>(PC, BeginWidgetClass);
    EndingUI = CreateWidget<UEndStoryUserWidget>(PC, EndingWidgetClass);
    BackToTitleUI();
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

void AHqGameModeBase::OnStatsChangedHandler(int32 Intel, int32 Charm, int32 Stamina, int32 Index)
{
    if (!PC) return;
    FName EndingRowName = NAME_None;
    if (!GameI->RandomRowNames.IsValidIndex(Index)) {
        EndingRowName = TEXT("Ending_TE");
    }
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

void AHqGameModeBase::BackToTitleUI()
{
    PC->BGMComponent->FadeOut(1.0f, 0.0f);
    PC->BGMComponent->SetSound(MainMusic);
    PC->BGMComponent->FadeIn(1.0f, 1.0f);
    PC->BGMComponent->Play();
    //UUserWidget* TitleUI = CreateWidget<UUserWidget>(PC, BeginWidgetClass);
    if (TitleUI)
    {
        TitleUI->AddToViewport(100); // 放在最前面
    }
}

void AHqGameModeBase::TriggerEnding(FName EndingRowName)
{
    if (!PC || !EndingTable || !EndingWidgetClass) return;
    //PC->BGMComponent->FadeOut(1.0f, 0.0f);
    if (PC->AudioComponent->IsPlaying())
    {
        PC->AudioComponent->Stop();
    }
    if (EndingRowName == TEXT("Ending_TE")) {
        UE_LOG(LogTemp, Warning, TEXT("[LRY] TE MUSIC"));
        PC->BGMComponent->SetSound(TEMusic);
    }else{
        UE_LOG(LogTemp, Warning, TEXT("[LRY] BE MUSIC"));
        PC->BGMComponent->SetSound(BEMusic);
    }
    //PC->BGMComponent->FadeIn(1.0f, 1.0f);

    UE_LOG(LogTemp, Warning, TEXT("[LRY] Attempting to trigger ending: %s"), *EndingRowName.ToString());
    FEndItem* Row = EndingTable->FindRow<FEndItem>(EndingRowName, TEXT("GetEnding"));
    if (Row)
    {
        //UEndStoryUserWidget* EndingUI = CreateWidget<UEndStoryUserWidget>(PC, EndingWidgetClass);
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
