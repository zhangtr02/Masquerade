// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskPlayerController.h"
#include "DialogueWidget.h"

void AMaskPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameI = GetGameInstance()->GetSubsystem<UHqGameInstanceSubsystem>();
	InitialTable();

	if (!DialogueWidgetClass) return;
	DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
	if (!DialogueWidget) return;

	DialogueWidget->AddToViewport(10);
	
	DialogueWidget->OnChoiceClicked.AddDynamic(this, &AMaskPlayerController::HandleChoiceClicked);
	DialogueWidget->OnTransitionFinished.AddDynamic(this, &AMaskPlayerController::HandleUITransitionFinished);
	
	DialogueWidget->MaxIntelligence = MaxIntelligence;
	DialogueWidget->MaxCharm = MaxCharm;
	DialogueWidget->MaxStamina = MaxStamina;
	
	DialogueWidget->SetStats(Intelligence, Charm, Stamina);
	
	ShowRandomEvent();
	DialogueWidget->PlayEventIn();
	
	SetShowMouseCursor(true);
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);
}

void AMaskPlayerController::InitialTable()
{
	if (GameI)
	{
		UE_LOG(LogTemp, Log, TEXT("[LRY]instance begin read table"));
		GameI->LoadConfig(AssetConfig);
		GameI->RandomRowNames = RandomTable->GetRowNames();
		RandomMax = GameI->RandomRowNames.Num();
		ShuffleEvent(GameI->RandomRowNames);
		GameI->ChracterARowNames = CharacterATable->GetRowNames();
	}
}

void AMaskPlayerController::ShuffleEvent(TArray<FName>& RowNameList)
{
	int32 Num = GameI->RandomRowNames.Num();
	for (int32 i = Num - 1; i > 0; --i)
	{
		// ���ѡһ��λ�ý��н���
		int32 j = FMath::RandRange(0, i);
		GameI->RandomRowNames.Swap(i, j);
	}
}

void AMaskPlayerController::HandleChoiceClicked(int32 ChoiceIndex)
{
	if (!DialogueWidget || bWaitingTransition) return;
	if (RandomIndex >= RandomMax) {
		UE_LOG(LogTemp, Warning, TEXT("[LRY] All random events have been shown."));
		return;
	}

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
	Charm = FMath::Clamp(Charm + Delta.Charm, 0, MaxCharm);
	Stamina = FMath::Clamp(Stamina + Delta.Stamina, 0, MaxStamina);

	DialogueWidget->SetStats(Intelligence, Charm, Stamina);
	
	ShowRandomEvent();
	DialogueWidget->PlayEventIn();
	
	PendingChoiceIndex = -1;
	FlowStage = EFlowStage::Idle;
	bWaitingTransition = false;
}

void AMaskPlayerController::ShowRandomEvent()
{
	if (!DialogueWidget) return;

	TArray<FName>& RowNamesRef = GameI->RandomRowNames;
	FName RowName = RowNamesRef[RandomIndex++];

	const FTableItemList* Row = RandomTable->FindRow<FTableItemList>(RowName, TEXT("PickRandomEvent"));
	if (!Row)
	{
		UE_LOG(LogTemp, Error, TEXT("[LRY] Failed to find row [%s] in RandomTable!"), *RowName.ToString());
		return;
	}
	
	CurrentEvent = *Row;
	DialogueWidget->SetEvent(
		CurrentEvent.Question,
		CurrentEvent.LeftChoice,
		CurrentEvent.RightChoice
	);


	UTexture2D* CachedImg = GameI->ImageCache.FindRef(Row->Character);

	if (CachedImg)
	{
		DialogueWidget->ChangeImage(CachedImg);
		UE_LOG(LogTemp, Log, TEXT("[LRY] Successfully applied texture for character: %s"), *Row->Character.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LRY] No cached image found for character [%s]. Check your LoadConfig or DataTable."), *Row->Character.ToString());
	}
}
