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
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> EventImage;
	
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> EventText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> LeftChoiceText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> RightChoiceText;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> LeftChoiceButton;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> RightChoiceButton;
	
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> IntelligenceBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> CharmBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> StaminaBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> IntelligenceRedGlowBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> CharmRedGlowBar;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> StaminaRedGlowBar;
	
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> EventOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> LeftChoiceOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> RightChoiceOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> PortraitOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> IntelligenceOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> CharmOverlay;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UWidget> StaminaOverlay;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> EventInAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> EventOutAnim;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> LeftChoiceOutAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> RightChoiceOutAnim;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> PortraitLeftOutAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> PortraitRightOutAnim;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> IntelligenceBumpAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> CharmBumpAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> StaminaBumpAnim;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> IntelligenceRedGlowAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> CharmRedGlowAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> StaminaRedGlowAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<USoundBase> ChoiceOutSFX;
	
	UPROPERTY(BlueprintReadOnly)
	int32 LastPickedIndex = 0;
	
	bool bStatsInitialized = false;
	int32 LastInt = 0;
	int32 LastCharm = 0;
	int32 LastStamina = 0;
	
	UPROPERTY(BlueprintAssignable) FOnDialogueChoiceClicked OnChoiceClicked;
	UPROPERTY(BlueprintAssignable) FOnDialogueTransitionFinished OnTransitionFinished;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxIntelligence = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxCharm = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxStamina = 100;
	
	UFUNCTION(BlueprintCallable)
	void SetEvent(const FText& BottomText, const FText& LeftText, const FText& RightText);
	
	UFUNCTION(BlueprintCallable)
	void SetStats(int32 Intelligence, int32 Charm, int32 Stamina);
	
	UFUNCTION(BlueprintCallable)
	void PlayEventIn(float StartAtTime = 0.f);
	
	UFUNCTION(BlueprintCallable)
	void PlayEventOut(int32 PickedIndex, float StartAtTime = 0.f);
	
	UFUNCTION(BlueprintCallable)
	void SetChoicesEnabled(bool bEnabled);
	UFUNCTION(BlueprintCallable)
	void ChangeImage(UTexture2D* Img);
	UFUNCTION(BlueprintCallable)
	void SetVisible();
	
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
	bool bPortraitLeftOutBound = false;
	bool bPortraitRightOutBound = false;
};
