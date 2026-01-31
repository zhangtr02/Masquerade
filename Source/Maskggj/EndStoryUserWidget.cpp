// Fill out your copyright notice in the Description page of Project Settings.


#include "EndStoryUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

FReply UEndStoryUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Error, TEXT("[LRY] Click End Story"));
	return FReply::Unhandled();
}

void UEndStoryUserWidget::SetupWidget(UTexture2D* Texture, FText Content)
{
	if (CGImage && Texture) CGImage->SetBrushFromTexture(Texture);
	if (StoryText) StoryText->SetText(Content);
}
