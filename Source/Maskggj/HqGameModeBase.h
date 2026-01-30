// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HqGameInstanceSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "HqGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MASKGGJ_API AHqGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskData")
	UDataTable* MyDataTable;
};
