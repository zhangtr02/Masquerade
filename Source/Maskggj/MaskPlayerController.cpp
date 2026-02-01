// Fill out your copyright notice in the Description page of Project Settings.

#include "MaskPlayerController.h"
#include "DialogueWidget.h"
#include "MainMenuWidget.h"
#include "EndStoryUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

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

	if (AudioComponent) AudioComponent->VolumeMultiplier = AudioVolume;
	if (BGMComponent)   BGMComponent->VolumeMultiplier = BGMVolume;
	if (SFXComponent)   SFXComponent->VolumeMultiplier = SFXVolume;

	if (SFXComponent && Click)
	{
		SFXComponent->SetSound(Click);
	}

	GameI = GetGameInstance()->GetSubsystem<UHqGameInstanceSubsystem>();
	InitialTable();
	
	ShowTitleUI();
}

void AMaskPlayerController::ApplyTitleInputMode()
{
	SetShowMouseCursor(true);
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);
}

void AMaskPlayerController::PlayMainBGM()
{
	if (!BGMComponent) return;
	if (!MainMusic) return;

	BGMComponent->FadeOut(0.3f, 0.0f);
	BGMComponent->SetSound(MainMusic);
	BGMComponent->FadeIn(0.3f, 1.0f);
	BGMComponent->Play();
}

void AMaskPlayerController::PlayEndingBGM(bool bIsTE)
{
	if (!BGMComponent) return;

	USoundBase* Target = bIsTE ? TEMusic : BEMusic;
	if (!Target) return;

	BGMComponent->FadeOut(0.2f, 0.0f);
	BGMComponent->SetSound(Target);
	BGMComponent->FadeIn(0.2f, 1.0f);
	BGMComponent->Play();
}

void AMaskPlayerController::ShowTitleUI()
{
	if (EndingUIInstance)
	{
		EndingUIInstance->RemoveFromParent();
		EndingUIInstance = nullptr;
	}
	
	if (TitleUIInstance)
	{
		TitleUIInstance->RemoveFromParent();
		TitleUIInstance = nullptr;
	}

	if (!BeginWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LRY] ShowTitleUI: BeginWidgetClass is null"));
		return;
	}

	TitleUIInstance = CreateWidget<UMainMenuWidget>(this, BeginWidgetClass);
	if (!TitleUIInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LRY] ShowTitleUI: CreateWidget failed"));
		return;
	}

	TitleUIInstance->AddToViewport(100);

	ApplyTitleInputMode();
	PlayMainBGM();
}

void AMaskPlayerController::ShowEndingUI(FName EndingRowName)
{
	CloseDialogueWidget();
	
	if (TitleUIInstance)
	{
		TitleUIInstance->RemoveFromParent();
		TitleUIInstance = nullptr;
	}
	
	if (EndingUIInstance)
	{
		EndingUIInstance->RemoveFromParent();
		EndingUIInstance = nullptr;
	}

	if (!EndingWidgetClass || !EndingTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[LRY] ShowEndingUI: Missing EndingWidgetClass or EndingTable"));
		return;
	}

	FEndItem* Row = EndingTable->FindRow<FEndItem>(EndingRowName, TEXT("ShowEndingUI"));
	if (!Row)
	{
		UE_LOG(LogTemp, Error, TEXT("[LRY] ShowEndingUI: Row '%s' not found"), *EndingRowName.ToString());
		return;
	}

	const bool bIsTE = (EndingRowName == TEXT("Ending_TE"));
	PlayEndingBGM(bIsTE);

	EndingUIInstance = CreateWidget<UEndStoryUserWidget>(this, EndingWidgetClass);
	if (!EndingUIInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[LRY] ShowEndingUI: CreateWidget failed"));
		return;
	}

	UTexture2D* EndImg = Row->Image.LoadSynchronous();
	EndingUIInstance->SetupWidget(EndImg, Row->EndStory);
	EndingUIInstance->AddToViewport(100);

	ApplyTitleInputMode();
}

void AMaskPlayerController::InitialDialogueWidget()
{
	if (TitleUIInstance)
	{
		TitleUIInstance->RemoveFromParent();
		TitleUIInstance = nullptr;
	}

	if (EndingUIInstance)
	{
		EndingUIInstance->RemoveFromParent();
		EndingUIInstance = nullptr;
	}

	RandomIndex = 0;
	Intelligence = 50;
	Charm = 50;
	Stamina = 50;

	bWaitingTransition = false;
	bIsGameOver = false;
	FlowStage = EFlowStage::Idle;

	if (!DialogueWidgetClass) return;

	DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
	if (!DialogueWidget) return;

	DialogueWidget->AddToViewport(10);
	
	DialogueWidget->OnChoiceClicked.RemoveDynamic(this, &AMaskPlayerController::HandleChoiceClicked);
	DialogueWidget->OnChoiceClicked.AddDynamic(this, &AMaskPlayerController::HandleChoiceClicked);

	DialogueWidget->OnTransitionFinished.RemoveDynamic(this, &AMaskPlayerController::HandleUITransitionFinished);
	DialogueWidget->OnTransitionFinished.AddDynamic(this, &AMaskPlayerController::HandleUITransitionFinished);

	DialogueWidget->OnStatsAnimationFinished.RemoveDynamic(this, &AMaskPlayerController::HandleStatsAnimFinished);
	DialogueWidget->OnStatsAnimationFinished.AddDynamic(this, &AMaskPlayerController::HandleStatsAnimFinished);

	DialogueWidget->MaxIntelligence = MaxIntelligence;
	DialogueWidget->MaxCharm = MaxCharm;
	DialogueWidget->MaxStamina = MaxStamina;

	DialogueWidget->SetStats(Intelligence, Charm, Stamina);
	ShowRandomEvent();
	DialogueWidget->PlayEventIn();

	ApplyGameInputMode();
}

void AMaskPlayerController::InitialTable()
{
	if (!GameI || !RandomTable) return;

	UE_LOG(LogTemp, Log, TEXT("[LRY] instance begin read table"));
	GameI->LoadConfig(AssetConfig);

	GameI->RandomRowNames = RandomTable->GetRowNames();
	CurrentRowNames = &GameI->RandomRowNames;
	ShuffleThreeStage(*CurrentRowNames);
}

void AMaskPlayerController::ShuffleThreeStage(TArray<FName>& RowNameList)
{
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

	if (!CurrentRowNames || !CurrentRowNames->IsValidIndex(RandomIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("[LRY] All story events shown."));
		OnStatsChanged.Broadcast(Intelligence, Charm, Stamina, RandomIndex);
		return;
	}

	if (SFXComponent) SFXComponent->Play();

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

	if (bIsGameOver) return;

	if (RandomIndex == ShuffleSettings.Random1_start)
	{
		DialogueWidget->SetVisible();
	}

	ShowRandomEvent();
	DialogueWidget->PlayEventIn();

	PendingChoiceIndex = -1;
	FlowStage = EFlowStage::Idle;
	bWaitingTransition = false;
}

void AMaskPlayerController::HandleStatsAnimFinished()
{
	if (OnStatsChanged.IsBound())
	{
		OnStatsChanged.Broadcast(Intelligence, Charm, Stamina, RandomIndex);
	}
	RandomIndex++;
}

void AMaskPlayerController::ShowRandomEvent()
{
	if (!DialogueWidget || !CurrentRowNames || !RandomTable || !CurrentRowNames->IsValidIndex(RandomIndex)) return;

	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}

	FName RowName = (*CurrentRowNames)[RandomIndex];

	const FTableItemList* Row = RandomTable->FindRow<FTableItemList>(RowName, TEXT("PickRandomEvent"));
	if (!Row)
	{
		UE_LOG(LogTemp, Error, TEXT("[LRY] Failed to find row [%s]"), *RowName.ToString());
		return;
	}

	CurrentEvent = *Row;
	DialogueWidget->SetEvent(CurrentEvent.Question, CurrentEvent.LeftChoice, CurrentEvent.RightChoice);
	
	if (GameI)
	{
		UTexture2D* CachedImg = GameI->ImageCache.FindRef(Row->Character);
		DialogueWidget->ChangeImage(CachedImg);
	}
	
	if (GameI && !Row->SFX.IsNone())
	{
		USoundBase* Sound = GameI->SoundCache.FindRef(Row->SFX);
		if (Sound && AudioComponent)
		{
			AudioComponent->SetSound(Sound);
			AudioComponent->Play();
		}
	}
}

void AMaskPlayerController::CloseDialogueWidget()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("[LRY] Remove dialogue ui"));
	}
}
