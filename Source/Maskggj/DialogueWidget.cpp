// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UDialogueWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (LeftChoiceButton)
	{
		LeftChoiceButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnLeftChoiceButtonClicked);
	}

	if (RightChoiceButton)
	{
		RightChoiceButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnRightChoiceButtonClicked);
	}
	
	SetImage(BackgroundImage);
	SetImage(PortraitImage);
	
	if (IntelligenceBar) IntelligenceBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CharmBar)        CharmBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (EnergyBar)       EnergyBar->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UDialogueWidget::SetImage(UImage* Image)
{
	if (IsValid(Image))
	{
		Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UDialogueWidget::SetEventText(const FText& BottomText, const FText& LeftText, const FText& RightText)
{
	if (EventText)
	{
		EventText->SetText(BottomText);
	}
	if (LeftChoiceText)
	{
		LeftChoiceText->SetText(LeftText);
	}
	if (RightChoiceText)
	{
		RightChoiceText->SetText(RightText);
	}
}

float UDialogueWidget::ToPercent(int32 Value, int32 MaxValue)
{
	if (MaxValue <= 0) return 0.f;
	return FMath::Clamp(static_cast<float>(Value) / static_cast<float>(MaxValue), 0.f, 1.f);
}

void UDialogueWidget::SetStats(int32 Intelligence, int32 Charm, int32 Energy)
{
	if (IntelligenceBar)
	{
		IntelligenceBar->SetPercent(ToPercent(Intelligence, MaxIntelligence));
	}
	if (CharmBar)
	{
		CharmBar->SetPercent(ToPercent(Charm, MaxCharm));
	}
	if (EnergyBar)
	{
		EnergyBar->SetPercent(ToPercent(Energy, MaxEnergy));
	}
}

void UDialogueWidget::OnLeftChoiceButtonClicked()
{
	OnChoiceClicked.Broadcast(0);
}

void UDialogueWidget::OnRightChoiceButtonClicked()
{
	OnChoiceClicked.Broadcast(1);
}