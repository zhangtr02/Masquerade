// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

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
	if (PortraitImage) PortraitImage->SetVisibility(ESlateVisibility::Hidden);
	if (EventImage) EventImage->SetVisibility(ESlateVisibility::Hidden);
	
	if (IntelligenceBar) IntelligenceBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CharmBar)        CharmBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (StaminaBar)       StaminaBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if (IntelligenceRedGlowBar) IntelligenceRedGlowBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CharmRedGlowBar) CharmRedGlowBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (StaminaRedGlowBar) StaminaRedGlowBar->SetVisibility(ESlateVisibility::HitTestInvisible);
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
	if (!PortraitImage) return;
	if (IsValid(Img)) {
		PortraitImage->SetBrushFromTexture(Img);
	}
}

void UDialogueWidget::SetVisible()
{
	PortraitImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	EventImage->SetVisibility(ESlateVisibility::HitTestInvisible);
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

void UDialogueWidget::SetStats(int32 Intelligence, int32 Charm, int32 Stamina)
{
	if (!bStatsInitialized)
	{
		bStatsInitialized = true;
		LastInt = Intelligence;
		LastCharm = Charm;
		LastStamina = Stamina;
	}
	
	if (IntelligenceBar && IntelligenceRedGlowBar)
	{
		IntelligenceBar->SetPercent(ToPercent(Intelligence, MaxIntelligence));
		IntelligenceRedGlowBar->SetPercent(ToPercent(Intelligence, MaxIntelligence));
	}
	if (CharmBar && CharmRedGlowBar)
	{
		CharmBar->SetPercent(ToPercent(Charm, MaxCharm));
		CharmRedGlowBar->SetPercent(ToPercent(Charm, MaxCharm));
	}
	if (StaminaBar && StaminaRedGlowBar)
	{
		StaminaBar->SetPercent(ToPercent(Stamina, MaxStamina));
		StaminaRedGlowBar->SetPercent(ToPercent(Stamina, MaxStamina));
	}
	
	const int32 DeltaInt = Intelligence - LastInt;
	if (DeltaInt > 0 && IntelligenceBumpAnim)
	{
		PlayAnimation(IntelligenceBumpAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	else if (DeltaInt < 0 && IntelligenceRedGlowAnim)
	{
		PlayAnimation(IntelligenceRedGlowAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	
	const int32 DeltaCharm = Charm - LastCharm;
	if (DeltaCharm > 0 && CharmBumpAnim)
	{
		PlayAnimation(CharmBumpAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	else if (DeltaCharm < 0 && CharmRedGlowAnim)
	{
		PlayAnimation(CharmRedGlowAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	
	const int32 DeltaStamina = Stamina - LastStamina;
	if (DeltaStamina > 0 && StaminaBumpAnim)
	{
		PlayAnimation(StaminaBumpAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	else if (DeltaStamina < 0 && StaminaRedGlowAnim)
	{
		PlayAnimation(StaminaRedGlowAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	
	LastInt = Intelligence;
	LastCharm = Charm;
	LastStamina = Stamina;
}

void UDialogueWidget::PlayEventIn(float StartAtTime)
{
	SetChoicesEnabled(false);
	CurrentStage = EDialogueAnimStage::EventIn;
	PendingFinishCount = 0;
	
	if (EventInAnim)
	{
		PlayAnimation(EventInAnim, StartAtTime, 1, EUMGSequencePlayMode::Forward, 1.f);
		BindAnimOnce(this, EventInAnim, bEventInBound, FName("OnAnimFinished"));
		PendingFinishCount++;
	}
	
	const int32 UnpickedIndex = 1 - LastPickedIndex;
	UWidgetAnimation* PortraitInViaOut = nullptr;
	bool* BoundFlag = nullptr;
	
	if (UnpickedIndex == 0)
	{
		PortraitInViaOut = PortraitRightOutAnim;
		BoundFlag = &bPortraitRightOutBound;
	}
	else
	{
		PortraitInViaOut = PortraitLeftOutAnim;
		BoundFlag = &bPortraitLeftOutBound;
	}
	
	if (PortraitInViaOut)
	{
		PlayAnimation(PortraitInViaOut, StartAtTime, 1, EUMGSequencePlayMode::Reverse, 1.f);
		BindAnimOnce(this, PortraitInViaOut, *BoundFlag, FName("OnAnimFinished"));
		PendingFinishCount++;
	}

	if (PendingFinishCount == 0)
	{
		SetChoicesEnabled(true);
		OnTransitionFinished.Broadcast();
	}
}

void UDialogueWidget::PlayEventOut(int32 PickedIndex, float StartAtTime)
{
	SetChoicesEnabled(false);
	CurrentStage = EDialogueAnimStage::EventOut;
	PendingFinishCount = 0;
	
	const bool bPortraitVisible = PortraitImage && PortraitImage->GetVisibility() != ESlateVisibility::Hidden;
	if (bPortraitVisible  && ChoiceOutSFX)
	{
		UGameplayStatics::PlaySound2D(this, ChoiceOutSFX);
	}
	
	LastPickedIndex = PickedIndex;
	
	if (EventOutAnim)
	{
		PlayAnimation(EventOutAnim, StartAtTime, 1, EUMGSequencePlayMode::Forward, 1.f);
		BindAnimOnce(this, EventOutAnim, bEventOutBound, FName("OnAnimFinished"));
		PendingFinishCount++;
	}
	
	UWidgetAnimation* PortraitOut = (PickedIndex == 0) ? PortraitLeftOutAnim : PortraitRightOutAnim;
	if (PortraitOut)
	{
		PlayAnimation(PortraitOut, StartAtTime, 1, EUMGSequencePlayMode::Forward, 1.f);

		if (PortraitOut == PortraitLeftOutAnim)
			BindAnimOnce(this, PortraitLeftOutAnim, bPortraitLeftOutBound, FName("OnAnimFinished"));
		else
			BindAnimOnce(this, PortraitRightOutAnim, bPortraitRightOutBound, FName("OnAnimFinished"));

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
