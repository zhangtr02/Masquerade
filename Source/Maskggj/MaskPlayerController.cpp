// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskPlayerController.h"
#include "DialogueWidget.h"

AMaskPlayerController::AMaskPlayerController()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	BGMComponent->bAutoActivate = false;
	SFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFXComponent"));
	SFXComponent->bAutoActivate = false;
}

void AMaskPlayerController::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->VolumeMultiplier = AudioVolume;
	BGMComponent->VolumeMultiplier = BGMVolume;
	SFXComponent->VolumeMultiplier = SFXVolume;

	GameI = GetGameInstance()->GetSubsystem<UHqGameInstanceSubsystem>();
	InitialTable();
	//InitialDialogueWidget();
}

void AMaskPlayerController::InitialDialogueWidget()
{
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

	//SetShowMouseCursor(true);
	//FInputModeGameAndUI Mode;
	//Mode.SetHideCursorDuringCapture(false);
	//SetInputMode(Mode);
}

void AMaskPlayerController::InitialTable()
{
	if (GameI)
	{
		UE_LOG(LogTemp, Log, TEXT("[LRY]instance begin read table"));
		GameI->LoadConfig(AssetConfig);

		GameI->RandomRowNames = RandomTable->GetRowNames();
		//存储表格rowname的指针
		CurrentRowNames = &GameI->RandomRowNames;
		ShuffleThreeStage(*CurrentRowNames);
	}
}

void AMaskPlayerController::ShuffleThreeStage(TArray<FName>& RowNameList) {

	ShuffleEvent(RowNameList, ShuffleSettings.Random1_start, ShuffleSettings.Random1_end);

	ShuffleEvent(RowNameList, ShuffleSettings.Random2_start, ShuffleSettings.Random2_end);

	ShuffleEvent(RowNameList, ShuffleSettings.Random3_start, ShuffleSettings.Random3_end);

	UE_LOG(LogTemp, Log, TEXT("[LRY] Three-section shuffle complete. Sections: [0-%d], [%d-%d], [%d-%d]"),
		ShuffleSettings.Random1_start, ShuffleSettings.Random1_end,
		ShuffleSettings.Random2_start, ShuffleSettings.Random2_end,
		ShuffleSettings.Random3_start, ShuffleSettings.Random3_end);
}

void AMaskPlayerController::ShuffleEvent(TArray<FName>& RowNameList, int32 StartIndex, int32 EndIndex)
{
	if (!RowNameList.IsValidIndex(StartIndex) || EndIndex <= StartIndex || EndIndex > RowNameList.Num())
	{
		return;
	}

	for (int32 i = EndIndex - 1; i > StartIndex; --i)
	{
		int32 j = FMath::RandRange(StartIndex, i);
		RowNameList.Swap(i, j);
	}
}

void AMaskPlayerController::HandleChoiceClicked(int32 ChoiceIndex)
{
	if (!DialogueWidget || bWaitingTransition) return;

	if (!CurrentRowNames->IsValidIndex(RandomIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("[LRY] All story events of this table have been shown."));
		return;
	}
	if (RandomIndex == ShuffleSettings.Random1_start)
	{
		DialogueWidget->SetVisible(); // 调用你 UW 的接口
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
	if (OnStatsChanged.IsBound())
	{
		OnStatsChanged.Broadcast(Intelligence, Charm, Stamina);
	}
	else {

	}

	// GameMode 监听到事件后，如果判定故事结束，会调用GameOver() 把 bIsGameOver 设为 true
	if (bIsGameOver)
	{
		return;
	}
	ShowRandomEvent();
	DialogueWidget->PlayEventIn();
	
	PendingChoiceIndex = -1;
	FlowStage = EFlowStage::Idle;
	bWaitingTransition = false;
}

void AMaskPlayerController::ShowRandomEvent()
{
	if (!DialogueWidget) return;

	if (AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}

	FName RowName = (*CurrentRowNames)[RandomIndex++];

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
		DialogueWidget->ChangeImage(CachedImg);
		UE_LOG(LogTemp, Warning, TEXT("[LRY] No cached image found for character [%s]. Check your LoadConfig or DataTable."), *Row->Character.ToString());
	}

	if (!Row->SFX.IsNone())
	{
		USoundBase* Sound = GameI->SoundCache.FindRef(Row->SFX);
		if (Sound) {
			AudioComponent->SetSound(Sound);
			AudioComponent->Play();
			UE_LOG(LogTemp, Log, TEXT("[LRY] Static Component playing: %s"), *Row->SFX.ToString());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("[LRY] Sound[%s] not found."), *Row->SFX.ToString())
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LRY] Row [%s] has no sound assigned."), *RowName.ToString());
	}
}

void AMaskPlayerController::CloseDialogueWidget()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
		//FInputModeGameOnly GameMode;
		//SetInputMode(GameMode);
		//SetShowMouseCursor(false);
	}
}
