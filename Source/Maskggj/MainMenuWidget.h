// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UImage;
class UButton;

/**
 * 
 */
UCLASS()
class MASKGGJ_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UImage> BackgroundImage;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UImage> TitleImage;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UImage> CandleImage;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> StartButton;
	UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> ExitButton;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> TitleFloatAnim;
	UPROPERTY(Transient, meta=(BindWidgetAnim)) TObjectPtr<UWidgetAnimation> FlickAnim;
	
	UPROPERTY(EditAnywhere)
	FVector2D CandleOffset = FVector2D(-150.f, -100.f);
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitClicked();
	
	UFUNCTION()
	void OnStartHovered();

	UFUNCTION()
	void OnStartUnhovered();

	UFUNCTION()
	void OnExitHovered();

	UFUNCTION()
	void OnExitUnhovered();
	
	void MoveCandleToWidget(UWidget* TargetWidget);
};
