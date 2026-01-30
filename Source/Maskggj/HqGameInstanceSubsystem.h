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
public:
    UPROPERTY()
    TMap<FName, FTableItemList> QuestionMap;

    // 玩家状态
    int32 Intelligence = 0;
    int32 Charm = 0;
    int32 Stamina = 100;

    void LoadAllRows(const UDataTable* Table, TMap<FName, FTableItemList>& OutMap);
};
