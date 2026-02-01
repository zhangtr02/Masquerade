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
    //StoryText->SetVisibility(ESlateVisibility::Hidden);
    FWidgetAnimationDynamicEvent EndEvent;
    EndEvent.BindDynamic(this, &UEndStoryUserWidget::OnFocusFinished);
    BindToAnimationFinished(Anim_Focus1, EndEvent);
    if (EndBtn)
    {
        // 绑定点击事件
        EndBtn->OnClicked.AddDynamic(this, &UEndStoryUserWidget::OnEndBtnClicked);
    }
    PlayAnimation(Anim_Focus1, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
}

void UEndStoryUserWidget::OnFocusFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("[LRY] End CG end"));
    StoryText->SetVisibility(ESlateVisibility::Visible);
}

void UEndStoryUserWidget::SetupWidget(UTexture2D* Texture, FText Content)
{
	if (CGImage && Texture) CGImage->SetBrushFromTexture(Texture);
	if (StoryText) StoryText->SetText(Content);
}
void UEndStoryUserWidget::OnEndBtnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("[LRY] Clicked End Button"));
    AHqGameModeBase* MyGM = Cast<AHqGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

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
