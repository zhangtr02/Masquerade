// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndStoryUserWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UWidgetAnimation;

UCLASS()
class MASKGGJ_API UEndStoryUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CGImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StoryText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EndBtn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_Focus1;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnFocusFinished();

	UFUNCTION()
	void OnEndBtnClicked();

	void SetupWidget(UTexture2D* Texture, FText Content);

private:
	FString FullStoryString;
	int32 CurrentCharIndex = 0;

	FTimerHandle TypewriterTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Logic")
	float TypewriterSpeed = 0.05f;

	void UpdateTypewriterText();
};
