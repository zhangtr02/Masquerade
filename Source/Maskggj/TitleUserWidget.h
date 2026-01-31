// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUserWidget.generated.h"

/**
 * 
 */

class UButton;
UCLASS()
class MASKGGJ_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BeginBtn;

	UFUNCTION()
	void OnBeginBtnClicked();

};
