// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUserWidget.h"
#include "Components/Button.h"
#include "HqGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void UTitleUserWidget::OnBeginBtnClicked()
{
    AHqGameModeBase* MyGM = Cast<AHqGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

 
    if (MyGM)
    {
        MyGM->StartMainUI();
        RemoveFromParent();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[LRY]无法获取到GameMode"));
    }
}

void UTitleUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (BeginBtn)
    {
        // 绑定点击事件
        BeginBtn->OnClicked.AddDynamic(this, &UTitleUserWidget::OnBeginBtnClicked);
    }
}
