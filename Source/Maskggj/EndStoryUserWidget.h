// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndStoryUserWidget.generated.h"

class UImage;
class UTextBlock;
UCLASS()
class MASKGGJ_API UEndStoryUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CGImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StoryText;
	UPROPERTY(EditAnywhere, Category = "Logic")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	void SetupWidget(UTexture2D* Texture, FText Content);
};
