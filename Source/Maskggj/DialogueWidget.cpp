// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
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
	SetImage(IntelligenceImage);
	SetImage(CharmImage);
	SetImage(EnergyImage);
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

void UDialogueWidget::SetStats(int32 Intelligence, int32 Charm, int32 Energy)
{
	if (IntelligenceText)
	{
		IntelligenceText->SetText(FText::AsNumber(Intelligence));
	}

	if (CharmText)
	{
		CharmText->SetText(FText::AsNumber(Charm));
	}

	if (EnergyText)
	{
		EnergyText->SetText(FText::AsNumber(Energy));
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