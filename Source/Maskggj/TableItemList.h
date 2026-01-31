// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "TableItemList.generated.h"


USTRUCT(BlueprintType)
struct MASKGGJ_API FChoice {
	GENERATED_BODY()
	FChoice() :Intelligence(0), Charm(0), Stamina(0){}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Intelligence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Charm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stamina;
};

USTRUCT(BlueprintType)
struct MASKGGJ_API FTableItemList : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Question;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LeftChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText RightChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FChoice LeftModify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FChoice RightModify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SFX;
};

USTRUCT(BlueprintType)
struct FFnameToLocation : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Portrait; // 软引用图片

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> Sound;
};

USTRUCT(BlueprintType)
struct FEndItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EndStory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Image; // 软引用图片

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> Sound;
};