// Fill out your copyright notice in the Description page of Project Settings.

#include "LostArcGameInstance.h"

ULostArcGameInstance::ULostArcGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/PlayerCharacter/LostArcCharacterData.LostArcCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER(*CharacterDataPath);

	ArcCharacterTable = DT_CHARACTER.Object;
}

void ULostArcGameInstance::Init()
{
	Super::Init();
}

FArcCharacterData* ULostArcGameInstance::GetArcCharacterData(int32 Level)
{
	return ArcCharacterTable->FindRow<FArcCharacterData>(*FString::FromInt(Level), TEXT(""));
}