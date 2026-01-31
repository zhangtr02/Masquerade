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
	
	if (BackgroundImage) BackgroundImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (PortraitImage) PortraitImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if (IntelligenceBar) IntelligenceBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CharmBar)        CharmBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (EnergyBar)       EnergyBar->SetVisibility(ESlateVisibility::HitTestInvisible);
}

static void BindAnimOnce(UUserWidget* Widget, UWidgetAnimation* Anim, bool& bBound, const FName FuncName)
{
	if (!Widget || !Anim || bBound) return;

	FWidgetAnimationDynamicEvent FinishedEvent;
	FinishedEvent.BindUFunction(Widget, FuncName);
	Widget->BindToAnimationFinished(Anim, FinishedEvent);
	bBound = true;
}

void UDialogueWidget::ChangeImage(UTexture2D* Img)
{
	if (IsValid(Img)) {
		PortraitImage->SetBrushFromTexture(Img);
	}
}

void UDialogueWidget::SetEvent(const FText& BottomText, const FText& LeftText, const FText& RightText)
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
	
	SetChoicesEnabled(true);
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

void UDialogueWidget::PlayEventIn(float StartAtTime)
{
	SetChoicesEnabled(false);
	CurrentStage = EDialogueAnimStage::EventIn;
	PendingFinishCount = 0;
	
	if (!EventInAnim)
	{
		SetChoicesEnabled(true);
		OnTransitionFinished.Broadcast();
		return;
	}
	
	PlayAnimation(EventInAnim, StartAtTime, 1, EUMGSequencePlayMode::Forward, 1.f);
	BindAnimOnce(this, EventInAnim, bEventInBound, FName("OnAnimFinished"));
	PendingFinishCount = 1;
}

void UDialogueWidget::PlayEventOut(int32 PickedIndex, float StartAtTime)
{
	SetChoicesEnabled(false);
	CurrentStage = EDialogueAnimStage::EventOut;
	PendingFinishCount = 0;
	
	if (EventOutAnim)
	{
		PlayAnimation(EventOutAnim, StartAtTime, 1, EUMGSequencePlayMode::Forward, 1.f);
		BindAnimOnce(this, EventOutAnim, bEventOutBound, FName("OnAnimFinished"));
		PendingFinishCount++;
	}
	
	UWidgetAnimation* UnselectedOut = (PickedIndex == 0) ? RightChoiceOutAnim : LeftChoiceOutAnim;
	if (UnselectedOut)
	{
		PlayAnimation(UnselectedOut, StartAtTime, 1, EUMGSequencePlayMode::Forward, 1.f);

		if (UnselectedOut == LeftChoiceOutAnim)
		{
			BindAnimOnce(this, LeftChoiceOutAnim, bLeftOutBound, FName("OnAnimFinished"));
		}
		else
		{
			BindAnimOnce(this, RightChoiceOutAnim, bRightOutBound, FName("OnAnimFinished"));
		}

		PendingFinishCount++;
	}
	
	if (PendingFinishCount == 0)
	{
		OnTransitionFinished.Broadcast();
	}
}

void UDialogueWidget::SetChoicesEnabled(bool bEnabled)
{
	if (LeftChoiceButton)  LeftChoiceButton->SetIsEnabled(bEnabled);
	if (RightChoiceButton) RightChoiceButton->SetIsEnabled(bEnabled);
}

void UDialogueWidget::OnLeftChoiceButtonClicked()
{
	OnChoiceClicked.Broadcast(0);
}

void UDialogueWidget::OnRightChoiceButtonClicked()
{
	OnChoiceClicked.Broadcast(1);
}

void UDialogueWidget::OnAnimFinished()
{
	PendingFinishCount = FMath::Max(0, PendingFinishCount - 1);

	if (PendingFinishCount == 0)
	{
		if (CurrentStage == EDialogueAnimStage::EventIn)
		{
			SetChoicesEnabled(true);
		}
		OnTransitionFinished.Broadcast();
	}
}
