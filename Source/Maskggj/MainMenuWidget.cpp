// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "HqGameModeBase.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

class AHqGameModeBase;
class UCanvasPanelSlot;

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
		StartButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnStartHovered);
		StartButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnStartUnhovered);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
		ExitButton->OnHovered.AddDynamic(this, &UMainMenuWidget::OnExitHovered);
		ExitButton->OnUnhovered.AddDynamic(this, &UMainMenuWidget::OnExitUnhovered);
	}
	
	if (CandleImage)
	{
		CandleImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (TitleFloatAnim)
	{
		// 播放并无限循环
		PlayAnimation(TitleFloatAnim, 0.f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	}
	
	if (FlickAnim)
	{
		// 播放并无限循环
		PlayAnimation(FlickAnim, 0.f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void UMainMenuWidget::OnStartClicked()
{
	if (StartClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, StartClickSFX);
	}
	
	if (!GetWorld())
	{
		return;
	}

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	AHqGameModeBase* HqGM = Cast<AHqGameModeBase>(GM);
	if (HqGM)
	{
		HqGM->StartMainUI();
		RemoveFromParent();
	}
}

void UMainMenuWidget::OnExitClicked()
{
	if (ExitClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, ExitClickSFX);
	}
	
	UKismetSystemLibrary::QuitGame(
		this,
		nullptr,
		EQuitPreference::Quit,
		true
	);
}

void UMainMenuWidget::OnStartHovered()
{
	MoveCandleToWidget(StartButton);
}

void UMainMenuWidget::OnStartUnhovered()
{
	if (CandleImage)
	{
		CandleImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainMenuWidget::OnExitHovered()
{
	MoveCandleToWidget(ExitButton);
}

void UMainMenuWidget::OnExitUnhovered()
{
	if (CandleImage)
	{
		CandleImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainMenuWidget::MoveCandleToWidget(UWidget* TargetWidget)
{
	if (!CandleImage || !TargetWidget)
	{
		return;
	}
	
	UCanvasPanelSlot* CandleSlot = Cast<UCanvasPanelSlot>(CandleImage->Slot);
	if (!CandleSlot)
	{
		return;
	}
	
	UCanvasPanelSlot* TargetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	if (!TargetSlot)
	{
		return;
	}

	const FVector2D TargetPos = TargetSlot->GetPosition();
	
	FVector2D NewPos = TargetPos + CandleOffset;

	CandleSlot->SetPosition(NewPos);
	
	CandleImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}
