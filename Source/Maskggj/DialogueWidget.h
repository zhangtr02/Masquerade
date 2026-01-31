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
class UProgressBar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceClicked, int32, ChoiceIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueTransitionFinished);

/**
 * 
 */

UENUM()
enum class EDialogueAnimStage : uint8
{
	None,
	EventIn,
	EventOut
};

UCLASS()
class MASKGGJ_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UImage> BackgroundImage;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UImage> PortraitImage;
	
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> EventText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> LeftChoiceText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> RightChoiceText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> LeftChoiceButton;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> RightChoiceButton;
	
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> IntelligenceText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> CharmText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> EnergyText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> IntelligenceBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> CharmBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> EnergyBar;
	
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> EventOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> LeftChoiceOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> RightChoiceOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> PortraitOverlay;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> EventInAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> EventOutAnim; 
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> LeftChoiceOutAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> RightChoiceOutAnim;
	
	UPROPERTY(BlueprintAssignable) FOnDialogueChoiceClicked OnChoiceClicked;
	UPROPERTY(BlueprintAssignable) FOnDialogueTransitionFinished OnTransitionFinished;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxIntelligence = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxCharm = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxEnergy = 100;
	
	UFUNCTION(BlueprintCallable)
	void SetEvent(const FText& BottomText, const FText& LeftText, const FText& RightText);
	
	UFUNCTION(BlueprintCallable)
	void SetStats(int32 Intelligence, int32 Charm, int32 Energy);
	
	UFUNCTION(BlueprintCallable)
	void PlayEventIn(float StartAtTime = 0.f);
	
	UFUNCTION(BlueprintCallable)
	void PlayEventOut(int32 PickedIndex, float StartAtTime = 0.f);
	
	UFUNCTION(BlueprintCallable)
	void SetChoicesEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void ChangeImage(UTexture2D* Img);
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void OnLeftChoiceButtonClicked();

	UFUNCTION()
	void OnRightChoiceButtonClicked();
	
	UFUNCTION()
	void OnAnimFinished();
	
	static float ToPercent(int32 Value, int32 MaxValue);
	
	EDialogueAnimStage CurrentStage = EDialogueAnimStage::None;
	int32 PendingFinishCount = 0;
	bool bEventInBound = false;
	bool bEventOutBound = false;
	bool bLeftOutBound = false;
	bool bRightOutBound = false;
};
