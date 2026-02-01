// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableItemList.h"
#include "Components/AudioComponent.h"
#include "HqGameInstanceSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "MaskPlayerController.generated.h"

class UDialogueWidget;
class UMainMenuWidget;
class UEndStoryUserWidget;
class UDataTable;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPlayerStatsChanged, int32, Intel, int32, Charm, int32, Stamina, int32, Index);

USTRUCT(BlueprintType)
struct FShuffleConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Random1_start = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Random1_end = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Random2_start = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Random2_end = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Random3_start = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Random3_end = 0;
};

UCLASS()
class MASKGGJ_API AMaskPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMaskPlayerController();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerStatsChanged OnStatsChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShuffleConfig ShuffleSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> RandomTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> AssetConfig;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainMenuWidget> BeginWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UEndStoryUserWidget> EndingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UDataTable> EndingTable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> SFXComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> BGMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> MainMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> TEMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> BEMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> Click;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float BGMVolume = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float AudioVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float SFXVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxIntelligence = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCharm = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStamina = 100;

	UPROPERTY()
	bool bIsGameOver = false;

	UFUNCTION()
	void GameOver() { bIsGameOver = true; }

	UFUNCTION()
	void InitialDialogueWidget();

	UFUNCTION()
	void CloseDialogueWidget();
	
	UFUNCTION(BlueprintCallable)
	void ShowTitleUI();
	
	UFUNCTION(BlueprintCallable)
	void ShowEndingUI(FName EndingRowName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UHqGameInstanceSubsystem> GameI;

	TArray<FName>* CurrentRowNames = nullptr;

	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;
	
	UPROPERTY()
	TObjectPtr<UMainMenuWidget> TitleUIInstance;

	UPROPERTY()
	TObjectPtr<UEndStoryUserWidget> EndingUIInstance;

	UFUNCTION()
	void HandleChoiceClicked(int32 ChoiceIndex);

	UFUNCTION()
	void HandleUITransitionFinished();

	UFUNCTION()
	void HandleStatsAnimFinished();

	UFUNCTION()
	void ShowRandomEvent();

	int32 Intelligence = 50;
	int32 Charm = 50;
	int32 Stamina = 50;

	FTableItemList CurrentEvent;

	bool bWaitingTransition = false;
	int32 PendingChoiceIndex = -1;

	enum class EFlowStage : uint8
	{
		Idle,
		WaitingExit
	};

	EFlowStage FlowStage = EFlowStage::Idle;
	int32 RandomIndex = 0;

	UFUNCTION()
	void InitialTable();

	void ShuffleEvent(TArray<FName>& RowNameList, int32 StartIndex, int32 EndIndex);

	UFUNCTION()
	void ShuffleThreeStage(TArray<FName>& RowNameList);

	void ApplyTitleInputMode();
	void PlayMainBGM();
	void PlayEndingBGM(bool bIsTE);
};
