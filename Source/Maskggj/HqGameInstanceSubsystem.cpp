// Fill out your copyright notice in the Description page of Project Settings.


#include "HqGameInstanceSubsystem.h"

void UHqGameInstanceSubsystem::LoadAllRows(const UDataTable* Table, TMap<FName, FTableItemList>& OutMap)
{
	if (!Table)
	{
		UE_LOG(LogTemp, Error, TEXT("[lry] LoadAllRows Failed: DataTable is NULL!"));
		return;
	}

	OutMap.Empty();
	static const FString ContextString(TEXT("[lry] Full Table Load"));

	TArray<FTableItemList*> RowPtrs;
	Table->GetAllRows<FTableItemList>(ContextString, RowPtrs);
	TArray<FName> RowNames = Table->GetRowNames();

	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		if (RowPtrs.IsValidIndex(i))
		{
			const FTableItemList& Data = *RowPtrs[i];
			OutMap.Add(RowNames[i], *RowPtrs[i]);
			FName RowID = RowNames[i];
			// 2. 打印全部成员信息
			UE_LOG(LogTemp, Log, TEXT("[lry]Row ID: %s"), *RowID.ToString());
			UE_LOG(LogTemp, Log, TEXT("[lry]  -> Character: %s"), *Data.Character.ToString());
			UE_LOG(LogTemp, Log, TEXT("[lry]  -> Name: %s"), *Data.Name.ToString());
			UE_LOG(LogTemp, Log, TEXT("[lry] -> Question: %s"), *Data.Question.ToString());
			UE_LOG(LogTemp, Log, TEXT("[lry]  -> Left Choice: %s"), *Data.LeftChoice.ToString());
			UE_LOG(LogTemp, Log, TEXT("[lry]  -> Right Choice: %s"), *Data.RightChoice.ToString());

			// 打印 LeftModify (FChoice 嵌套结构)
			UE_LOG(LogTemp, Log, TEXT("[lry]  -> LeftModify: [Int:%d, Charm:%d, Stamina:%d]"),
				Data.LeftModify.Intelligence, Data.LeftModify.Charm, Data.LeftModify.Stamina);

			// 打印 RightModify (FChoice 嵌套结构)
			UE_LOG(LogTemp, Log, TEXT("[lry]  -> RightModify: [Int:%d, Charm:%d, Stamina:%d]"),
				Data.RightModify.Intelligence, Data.RightModify.Charm, Data.RightModify.Stamina);

			UE_LOG(LogTemp, Log, TEXT("[lry]  -> SFX: %s"), *Data.SFX.ToString());
			UE_LOG(LogTemp, Log, TEXT("[lry]------------------------------------------------------------"));
		}
	}
}
