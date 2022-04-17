// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "LostArcGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FArcCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FArcCharacterData() :Level(1), Nextexp(30), Maxhp(100.0f), Maxmp(100.0f), Attack(10.0f), Defense(1.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Nextexp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Maxhp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Maxmp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Defense;
};

UCLASS()
class LOSTARC_API ULostArcGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULostArcGameInstance();
	virtual void Init() override;
	FArcCharacterData* GetArcCharacterData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* ArcCharacterTable;
};
