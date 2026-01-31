// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HqGameInstanceSubsystem.h"
#include "MaskPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "HqGameModeBase.generated.h"

UCLASS()
class MASKGGJ_API AHqGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> MainMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<UDataTable> EndingTable;
	// 在编辑器里指定结局 UI 的类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<class UEndStoryUserWidget> EndingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<class UTitleUserWidget> BeginWidgetClass;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void TriggerEnding(FName EndingRowName);
	UFUNCTION()
	void OnStatsChangedHandler(int32 Intel, int32 Charm, int32 Stamina);

	UFUNCTION()
	void StartMainUI();

private:
	AMaskPlayerController* PC;
	UHqGameInstanceSubsystem* GameI;
};
