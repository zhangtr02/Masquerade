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
	
	DialogueWidget->MaxIntelligence = MaxIntelligence;
	DialogueWidget->MaxCharm = MaxCharm;
	DialogueWidget->MaxEnergy = MaxEnergy;
	
	DialogueWidget->SetStats(Intelligence, Charm, Energy);
	
	ShowRandomEvent();
	
	SetShowMouseCursor(true);
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);
}

void AMaskPlayerController::HandleChoiceClicked(int32 ChoiceIndex)
{
	const FChoice& Delta = (ChoiceIndex == 0) ? CurrentEvent.LeftModify : CurrentEvent.RightModify;
	Intelligence = FMath::Clamp(Intelligence + Delta.Intelligence, 0, MaxIntelligence);
	Charm        = FMath::Clamp(Charm + Delta.Charm, 0, MaxCharm);
	Energy       = FMath::Clamp(Energy + Delta.Stamina, 0, MaxEnergy);
	
	if (DialogueWidget)
	{
		DialogueWidget->SetStats(Intelligence, Charm, Energy);
	}
	
	ShowRandomEvent();
}

void AMaskPlayerController::ShowRandomEvent()
{
	if (!DialogueWidget || !EventTable) return;
	const TArray<FName> RowNames = EventTable->GetRowNames();
	if (RowNames.Num() == 0) return;
	
	const int32 Index = FMath::RandRange(0, RowNames.Num() - 1);
	const FName PickedRow = RowNames[Index];
	const FTableItemList* Row = EventTable->FindRow<FTableItemList>(PickedRow, TEXT("PickRandomEvent"));
	if (!Row) return;
	
	CurrentEvent = *Row;
	DialogueWidget->SetEventText(
		CurrentEvent.Question,
		CurrentEvent.LeftChoice,
		CurrentEvent.RightChoice
	);
}
