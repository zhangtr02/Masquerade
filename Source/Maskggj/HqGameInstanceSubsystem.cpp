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

		// --- 2. 处理 PortraitMask ---
		if (!RowPtrs[i]->PortraitMask.IsNull())
		{
			UTexture2D* LoadedTextureMask = RowPtrs[i]->PortraitMask.LoadSynchronous();
			if (LoadedTextureMask)
			{
				FName MaskName = FName(*(CurrentRowName.ToString() + TEXT("_mask")));
				ImageCache.Add(MaskName, LoadedTextureMask);
				UE_LOG(LogTemp, Log, TEXT("[LRY] Loaded Mask for Row [%s] as [%s]"), *CurrentRowName.ToString(), *MaskName.ToString());
			}
		}

		// --- 3. 处理 Sound ---
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
//void UHqGameInstanceSubsystem::LoadConfig(const UDataTable* Table)
//{
//	if (!Table)
//	{
//		UE_LOG(LogTemp, Error, TEXT("[lry] LoadConfig Failed: DataTable is NULL!"));
//		return;
//	}
//	TArray<FFnameToLocation*> RowPtrs;
//	static const FString ContextString(TEXT("[lry] Full Table Load"));
//	Table->GetAllRows<FFnameToLocation>(ContextString, RowPtrs);
//	TArray<FName> RowNames = Table->GetRowNames();
//
//	for (int32 i = 0; i < RowNames.Num(); ++i)
//	{
//		if (RowPtrs.IsValidIndex(i))
//		{
//			if (RowPtrs[i]->Portrait != NULL) {
//				TSoftObjectPtr<UTexture2D> Img = RowPtrs[i]->Portrait;
//				UTexture2D* LoadedTexture = Img.LoadSynchronous();
//				ImageCache.Add(RowNames[i], LoadedTexture);
//			}
//			else if (RowPtrs[i]->PortraitMask != NULL) {
//				TSoftObjectPtr<UTexture2D> ImgMask = RowPtrs[i]->PortraitMask;
//				UTexture2D* LoadedTextureMask = ImgMask.LoadSynchronous();
//				FName MaskName = FName(*(RowNames[i].ToString() + TEXT("_mask")));
//				ImageCache.Add(RowNames[i], LoadedTextureMask);
//			}
//			else if (RowPtrs[i]->Sound != NULL) {
//				USoundBase* LoadedSound = RowPtrs[i]->Sound.LoadSynchronous();
//				if (LoadedSound)
//				{
//					SoundCache.Add(RowNames[i], LoadedSound);
//				}
//			}
//		}
//	}
//}
