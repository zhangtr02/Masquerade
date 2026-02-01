// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HqGameModeBase.generated.h"

class AMaskPlayerController;
class UHqGameInstanceSubsystem;

UCLASS()
class MASKGGJ_API AHqGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void OnStatsChangedHandler(int32 Intel, int32 Charm, int32 Stamina, int32 Index);

	UFUNCTION()
	void StartMainUI();

private:
	UPROPERTY()
	TObjectPtr<AMaskPlayerController> PC;

	UPROPERTY()
	TObjectPtr<UHqGameInstanceSubsystem> GameI;
};
