// Fill out your copyright notice in the Description page of Project Settings.


#include "HqGameInstanceSubsystem.h"



void UHqGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
}

void UHqGameInstanceSubsystem::LoadConfig(const UDataTable* Table)
{
	if (!Table)
	{
		UE_LOG(LogTemp, Error, TEXT("[LRY] LoadConfig Failed: DataTable is NULL!"));
		return;
	}

	TArray<FFnameToLocation*> RowPtrs;
	static const FString ContextString(TEXT("[LRY] Full Table Load"));
	Table->GetAllRows<FFnameToLocation>(ContextString, RowPtrs);
	TArray<FName> RowNames = Table->GetRowNames();

	UE_LOG(LogTemp, Warning, TEXT("[LRY] Starting LoadConfig. Total Rows: %d"), RowNames.Num());

	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		if (!RowPtrs.IsValidIndex(i)) continue;

		FName CurrentRowName = RowNames[i];
		UE_LOG(LogTemp, Log, TEXT("[LRY] Processing Row [%s]..."), *CurrentRowName.ToString());
		// --- 1. 处理 Portrait ---
		if (!RowPtrs[i]->Portrait.IsNull())
		{
			UTexture2D* LoadedTexture = RowPtrs[i]->Portrait.LoadSynchronous();
			if (LoadedTexture)
			{
				ImageCache.Add(CurrentRowName, LoadedTexture);
				UE_LOG(LogTemp, Log, TEXT("[LRY] Loaded Portrait for Row [%s]"), *CurrentRowName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[LRY] Failed to load Portrait for Row [%s]! Path: %s"),
					*CurrentRowName.ToString(), *RowPtrs[i]->Portrait.ToString());
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("[LRY] Row [%s]: Portrait field is EMPTY in DataTable."), *CurrentRowName.ToString());
		}
		if (!RowPtrs[i]->Sound.IsNull())
		{
			USoundBase* LoadedSound = RowPtrs[i]->Sound.LoadSynchronous();
			if (LoadedSound)
			{
				SoundCache.Add(CurrentRowName, LoadedSound);
				UE_LOG(LogTemp, Log, TEXT("[LRY] Loaded Sound for Row [%s]"), *CurrentRowName.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[LRY] Failed to load Sound for Row [%s]!"), *CurrentRowName.ToString());
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[LRY] LoadConfig Finished. ImageCache: %d, SoundCache: %d"),
		ImageCache.Num(), SoundCache.Num());
}