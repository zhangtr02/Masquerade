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
	DialogueWidget->OnTransitionFinished.AddDynamic(this, &AMaskPlayerController::HandleUITransitionFinished);
	
	DialogueWidget->MaxIntelligence = MaxIntelligence;
	DialogueWidget->MaxCharm = MaxCharm;
	DialogueWidget->MaxEnergy = MaxEnergy;
	
	DialogueWidget->SetStats(Intelligence, Charm, Energy);
	
	ShowRandomEvent();
	DialogueWidget->PlayEventIn();
	
	SetShowMouseCursor(true);
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);
}

void AMaskPlayerController::HandleChoiceClicked(int32 ChoiceIndex)
{
	if (!DialogueWidget || bWaitingTransition) return;

	bWaitingTransition = true;
	PendingChoiceIndex = ChoiceIndex;

	FlowStage = EFlowStage::WaitingExit;
	
	DialogueWidget->PlayEventOut(ChoiceIndex);
}

void AMaskPlayerController::HandleUITransitionFinished()
{
	if (!DialogueWidget) return;
	
	if (FlowStage != EFlowStage::WaitingExit) return;
	
	const FChoice& Delta = (PendingChoiceIndex == 0) ? CurrentEvent.LeftModify : CurrentEvent.RightModify;

	Intelligence = FMath::Clamp(Intelligence + Delta.Intelligence, 0, MaxIntelligence);
	Charm        = FMath::Clamp(Charm + Delta.Charm, 0, MaxCharm);
	Energy       = FMath::Clamp(Energy + Delta.Stamina, 0, MaxEnergy);

	DialogueWidget->SetStats(Intelligence, Charm, Energy);
	
	ShowRandomEvent();
	DialogueWidget->PlayEventIn();
	
	PendingChoiceIndex = -1;
	FlowStage = EFlowStage::Idle;
	bWaitingTransition = false;
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
	DialogueWidget->SetEvent(
		CurrentEvent.Question,
		CurrentEvent.LeftChoice,
		CurrentEvent.RightChoice
	);
}
