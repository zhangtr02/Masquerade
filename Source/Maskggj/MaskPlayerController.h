// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableItemList.h"
#include "Components/AudioComponent.h"
#include "HqGameInstanceSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "MaskPlayerController.generated.h"

class UDialogueWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPlayerStatsChanged, int32, Intel, int32, Charm, int32, Stamina, int32, Index);
USTRUCT(BlueprintType)
struct FShuffleConfig
{
	GENERATED_BODY()

	// 每一段想要抽取的行数
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
	AMaskPlayerController();
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerStatsChanged OnStatsChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShuffleConfig ShuffleSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> RandomTable;

	//存储位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> AssetConfig;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;

	//事件音
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> AudioComponent;
	//点击音效等
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> SFXComponent;
	//背景音乐
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	TObjectPtr<UAudioComponent> BGMComponent;

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
	
protected:
	virtual void BeginPlay() override;
	
private:

	UHqGameInstanceSubsystem* GameI;
	TArray<FName>* CurrentRowNames;
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;

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
};
