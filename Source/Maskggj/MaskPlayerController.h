// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableItemList.h"
#include "GameFramework/PlayerController.h"
#include "MaskPlayerController.generated.h"

class UDialogueWidget;

/**
 * 
 */
UCLASS()
class MASKGGJ_API AMaskPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> EventTable;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxIntelligence = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCharm = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxEnergy = 100;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;
	
	UFUNCTION()
	void HandleChoiceClicked(int32 ChoiceIndex);
	
	UFUNCTION()
	void HandleUITransitionFinished();
	
	UFUNCTION()
	void ShowRandomEvent();
	
	int32 Intelligence = 30;
	int32 Charm = 40;
	int32 Energy = 50;
	
	FTableItemList CurrentEvent;
	
	bool bWaitingTransition = false;
	int32 PendingChoiceIndex = -1;

	enum class EFlowStage : uint8
	{
		Idle,
		WaitingExit
	};
	
	EFlowStage FlowStage = EFlowStage::Idle;
};
