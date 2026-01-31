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
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;
	
	UFUNCTION()
	void HandleChoiceClicked(int32 ChoiceIndex);
	
	UFUNCTION()
	void ShowRandomEvent();
	
	int32 Intelligence = 0;
	int32 Charm = 0;
	int32 Energy = 0;
	
	FTableItemList CurrentEvent;
};
