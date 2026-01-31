// Fill out your copyright notice in the Description page of Project Settings.


#include "HqGameModeBase.h"
#include "MaskPlayerController.h"

void AHqGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    AMaskPlayerController* PC = Cast<AMaskPlayerController>(GetWorld()->GetPlayerControllerIterator()->Get());
    PC->BGMComponent->SetSound(MainMusic);
    PC->BGMComponent->Play();

}
