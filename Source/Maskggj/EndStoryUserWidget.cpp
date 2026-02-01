// Fill out your copyright notice in the Description page of Project Settings.


#include "EndStoryUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "HqGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void UEndStoryUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FWidgetAnimationDynamicEvent EndEvent;
    EndEvent.BindDynamic(this, &UEndStoryUserWidget::OnFocusFinished);
    BindToAnimationFinished(Anim_Focus1, EndEvent);
    if (EndBtn)
    {
        EndBtn->OnClicked.AddDynamic(this, &UEndStoryUserWidget::OnEndBtnClicked);
    }
    PlayAnimation(Anim_Focus1, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);

    GetWorld()->GetTimerManager().SetTimer(
        TypewriterTimerHandle,
        this,
        &UEndStoryUserWidget::UpdateTypewriterText,
        TypewriterSpeed,
        true
    );
}

void UEndStoryUserWidget::OnFocusFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("[LRY] End CG end"));
    StoryText->SetVisibility(ESlateVisibility::Visible);
}

void UEndStoryUserWidget::SetupWidget(UTexture2D* Texture, FText Content)
{
	if (CGImage && Texture) CGImage->SetBrushFromTexture(Texture);

    FullStoryString = Content.ToString();
    CurrentCharIndex = 0;

    if (StoryText) StoryText->SetText(FText::GetEmpty());
}

void UEndStoryUserWidget::UpdateTypewriterText()
{
    if (CurrentCharIndex < FullStoryString.Len())
    {
        CurrentCharIndex++;

        // 换行
        if (FullStoryString.IsValidIndex(CurrentCharIndex) &&
            FullStoryString[CurrentCharIndex - 1] == '\\' &&
            FullStoryString[CurrentCharIndex] == 'n')
        {
            CurrentCharIndex++;
        }

        FString DisplayString = FullStoryString.Left(CurrentCharIndex);

        // 将转义的 \n 替换为换行
        StoryText->SetText(FText::FromString(DisplayString.Replace(TEXT("\\n"), TEXT("\n"))));
    }
    else
    {
        // 打字结束，清理定时器
        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);
    }
}

void UEndStoryUserWidget::OnEndBtnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("[LRY] Clicked End Button"));
    AHqGameModeBase* MyGM = Cast<AHqGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GetWorld()->GetTimerManager().IsTimerActive(TypewriterTimerHandle))
    {
        UE_LOG(LogTemp, Warning, TEXT("[LRY] Typewriter Skipped"));

        GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);

        if (StoryText)
        {
            StoryText->SetText(FText::FromString(FullStoryString.Replace(TEXT("\\n"), TEXT("\n"))));
        }

        CurrentCharIndex = FullStoryString.Len();
        return;
    }

    if (MyGM)
    {
        MyGM->BackToTitleUI();
        RemoveFromParent();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[LRY]无法获取到GameMode"));
    }
}
