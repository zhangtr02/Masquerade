// Fill out your copyright notice in the Description page of Project Settings.

#include "EndStoryUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MaskPlayerController.h"

void UEndStoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (EndBtn)
	{
		EndBtn->OnClicked.RemoveDynamic(this, &UEndStoryUserWidget::OnEndBtnClicked);
		EndBtn->OnClicked.AddDynamic(this, &UEndStoryUserWidget::OnEndBtnClicked);
	}

	if (Anim_Focus1)
	{
		FWidgetAnimationDynamicEvent EndEvent;
		EndEvent.BindDynamic(this, &UEndStoryUserWidget::OnFocusFinished);
		
		BindToAnimationFinished(Anim_Focus1, EndEvent);

		PlayAnimation(Anim_Focus1, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TypewriterTimerHandle);
		World->GetTimerManager().SetTimer(
			TypewriterTimerHandle,
			this,
			&UEndStoryUserWidget::UpdateTypewriterText,
			TypewriterSpeed,
			true
		);
	}
}

void UEndStoryUserWidget::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TypewriterTimerHandle);
	}
	Super::NativeDestruct();
}

void UEndStoryUserWidget::OnFocusFinished()
{
	if (StoryText)
	{
		StoryText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEndStoryUserWidget::SetupWidget(UTexture2D* Texture, FText Content)
{
	if (CGImage && Texture)
	{
		CGImage->SetBrushFromTexture(Texture);
	}

	FullStoryString = Content.ToString();
	CurrentCharIndex = 0;

	if (StoryText)
	{
		StoryText->SetText(FText::GetEmpty());
	}
}

void UEndStoryUserWidget::UpdateTypewriterText()
{
	if (!StoryText) return;

	if (CurrentCharIndex < FullStoryString.Len())
	{
		CurrentCharIndex++;
		
		if (FullStoryString.IsValidIndex(CurrentCharIndex) &&
			FullStoryString[CurrentCharIndex - 1] == '\\' &&
			FullStoryString[CurrentCharIndex] == 'n')
		{
			CurrentCharIndex++;
		}

		FString DisplayString = FullStoryString.Left(CurrentCharIndex);
		StoryText->SetText(FText::FromString(DisplayString.Replace(TEXT("\\n"), TEXT("\n"))));
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TypewriterTimerHandle);
		}
	}
}

void UEndStoryUserWidget::OnEndBtnClicked()
{
	if (UWorld* World = GetWorld())
	{
		if (World->GetTimerManager().IsTimerActive(TypewriterTimerHandle))
		{
			World->GetTimerManager().ClearTimer(TypewriterTimerHandle);

			if (StoryText)
			{
				StoryText->SetText(FText::FromString(FullStoryString.Replace(TEXT("\\n"), TEXT("\n"))));
			}

			CurrentCharIndex = FullStoryString.Len();
			return;
		}
	}
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMaskPlayerController* MPC = Cast<AMaskPlayerController>(PC))
		{
			MPC->ShowTitleUI();
		}
	}
	
	RemoveFromParent();
}
