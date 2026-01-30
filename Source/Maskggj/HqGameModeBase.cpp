// Fill out your copyright notice in the Description page of Project Settings.


#include "HqGameModeBase.h"

void AHqGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    // 1. 获取子系统
    UHqGameInstanceSubsystem* DataSys = GetGameInstance()->GetSubsystem<UHqGameInstanceSubsystem>();

    if (DataSys)
    {
        UE_LOG(LogTemp, Log, TEXT("[LRY]instance begin read table"));
        DataSys->LoadAllRows(MyDataTable, DataSys->QuestionMap);
    }
}
