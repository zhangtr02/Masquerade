// Fill out your copyright notice in the Description page of Project Settings.


#include "EndStoryUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"

void UEndStoryUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (EndBtn)
    {
        // 绑定点击事件
        EndBtn->OnClicked.AddDynamic(this, &UEndStoryUserWidget::OnEndBtnClicked);
    }
}

void UEndStoryUserWidget::SetupWidget(UTexture2D* Texture, FText Content)
{
	if (CGImage && Texture) CGImage->SetBrushFromTexture(Texture);
	if (StoryText) StoryText->SetText(Content);
}
void UEndStoryUserWidget::OnEndBtnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("[LRY] Clicked End Button via C++!"));
}
