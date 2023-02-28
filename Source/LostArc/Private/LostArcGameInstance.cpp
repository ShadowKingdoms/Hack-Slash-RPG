// Fill out your copyright notice in the Description page of Project Settings.

#include "LostArcGameInstance.h"

ULostArcGameInstance::ULostArcGameInstance()
{
	FString CharacterDataPaths = TEXT("DataTable'/Game/GameData/PlayerCharacter/ArcCharacterData.ArcCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTERSTAT(*CharacterDataPaths);
	ArcCharacterStatTable = DT_CHARACTERSTAT.Object;
	KillCount = 0;
}

void ULostArcGameInstance::Init()
{
	Super::Init();
}	

FArcCharacterStatData* ULostArcGameInstance::GetArcCharacterStatData(int32 Level)
{
	return ArcCharacterStatTable->FindRow<FArcCharacterStatData>(*FString::FromInt(Level), TEXT(""));
}

void ULostArcGameInstance::PlusKillCount()
{
	KillCount++;
}

int32 ULostArcGameInstance::GetKillCount()
{
	return KillCount;
}
