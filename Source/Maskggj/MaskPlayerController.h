// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableItemList.h"
#include "HqGameInstanceSubsystem.h"
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
	TObjectPtr<UDataTable> RandomTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> CharacterATable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> CharacterBTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> CharacterCTable;

	//资源地址
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> AssetConfig;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
protected:
	virtual void BeginPlay() override;
	
private:

	UHqGameInstanceSubsystem* GameI;
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
	int32 RandomIndex = 0;
	int32 RandomMax = 0;

	UFUNCTION()
	void InitialTable();
	UFUNCTION()
	void ShuffleEvent(TArray<FName>& RowNameList);
};
