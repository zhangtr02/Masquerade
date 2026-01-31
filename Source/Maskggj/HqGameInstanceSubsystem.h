// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableItemList.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HqGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MASKGGJ_API UHqGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
public:
    UPROPERTY()
    TArray<FName> RandomRowNames;

    UPROPERTY()
    TArray<FName> ChracterARowNames;
    UPROPERTY()
    TArray<FName> ChracterBRowNames;

    UPROPERTY()
    TArray<FName> ChracterCRowNames;

    UPROPERTY()
    TMap<FName, UTexture2D*> ImageCache;

    TMap<FName, USoundBase*> SoundCache;

    // 玩家状态
    int32 Intelligence = 0;
    int32 Charm = 0;
    int32 Stamina = 100;

    void LoadConfig(const UDataTable* Table);
};
