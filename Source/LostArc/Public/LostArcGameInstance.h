// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "LostArcGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FArcCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FArcCharacterStatData() : Level(1), MaxHP(500.f), MaxMP(300.0f), Attack(10.f), Defense(1.0f), Critical(10.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Critical;
};

UCLASS()
class LOSTARC_API ULostArcGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULostArcGameInstance();
	virtual void Init() override;
	FArcCharacterStatData* GetArcCharacterStatData(int32 Level);

private:

	UPROPERTY()
	UDataTable* ArcCharacterStatTable;
};
