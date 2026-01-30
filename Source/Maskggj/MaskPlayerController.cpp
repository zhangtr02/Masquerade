// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskPlayerController.h"

#include "DialogueWidget.h"

void AMaskPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!DialogueWidgetClass) return;

	DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
	if (!DialogueWidget) return;

	DialogueWidget->AddToViewport(10);
	
	DialogueWidget->OnChoiceClicked.AddDynamic(this, &AMaskPlayerController::HandleChoiceClicked);
	
	DialogueWidget->SetStats(Intelligence, Charm, Energy);
	
	DialogueWidget->SetEventText(
		FText::FromString(TEXT("这是一个测试事件：点击左/右给属性加点")),
		FText::FromString(TEXT("左：+1智力")),
		FText::FromString(TEXT("右：+1魅力"))
	);
	
	SetShowMouseCursor(true);
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);
}

void AMaskPlayerController::HandleChoiceClicked(int32 ChoiceIndex)
{
	if (ChoiceIndex == 0)
	{
		Intelligence += 1;
	}
	else
	{
		Charm += 1;
	}
	
	if (DialogueWidget)
	{
		DialogueWidget->SetStats(Intelligence, Charm, Energy);
	}
}