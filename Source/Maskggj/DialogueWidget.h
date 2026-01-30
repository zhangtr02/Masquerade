// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UTexture2D;
class UWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceClicked, int32, ChoiceIndex);

/**
 * 
 */
UCLASS()
class MASKGGJ_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> BackgroundImage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PortraitImage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> EventText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeftChoiceButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> LeftChoiceText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RightChoiceButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RightChoiceText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IntelligenceImage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> IntelligenceText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> CharmImage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CharmText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> EnergyImage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> EnergyText;
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueChoiceClicked OnChoiceClicked;
	
	UFUNCTION(BlueprintCallable)
	void SetImage(UImage* Image);
	
	UFUNCTION(BlueprintCallable)
	void SetEventText(
		const FText& BottomText,
		const FText& LeftText,
		const FText& RightText
	);
	
	UFUNCTION(BlueprintCallable)
	void SetStats(int32 Intelligence, int32 Charm, int32 Energy);
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void OnLeftChoiceButtonClicked();

	UFUNCTION()
	void OnRightChoiceButtonClicked();
	
};
