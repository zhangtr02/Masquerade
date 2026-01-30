// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;
	
	UFUNCTION()
	void HandleChoiceClicked(int32 ChoiceIndex);
	
	int32 Intelligence = 2;
	int32 Charm = 4;
	int32 Energy = 6;
};
