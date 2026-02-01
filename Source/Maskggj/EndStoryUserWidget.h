// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndStoryUserWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

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
	UPROPERTY(EditAnywhere, Category = "Logic")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) TObjectPtr<UWidgetAnimation> Anim_Focus1;

	UFUNCTION()
	void OnEndBtnClicked();

	virtual void NativeConstruct() override;

	void OnFocusFinished();
public:
	void SetupWidget(UTexture2D* Texture, FText Content);
};
