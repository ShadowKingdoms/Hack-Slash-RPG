// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LostArcGameInstance.h"
#include "Components/ActorComponent.h"
#include "LostArcCharacterStatComponent.generated.h"

UENUM(BlueprintType)
enum EAttributeType
{
	HP UMETA(DisplayName = "HP"),
	MP UMETA(DisplayName = "MP"),
	ATK UMETA(DisplayName = "ATK"),
	DEF UMETA(DisplayName = "DEF"),
	EXP UMETA(DisplayName = "EXP"),
	LVL UMETA(DisplayName = "LVL")
};

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnProgressBarDelegate, EAttributeType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOSTARC_API ULostArcCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULostArcCharacterStatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetDamage(float NewDamage);
	float GetCurrentAttributeValue(EAttributeType Type);
	float GetMaxAttributeValue(EAttributeType Type);
	float GetCurrentAttributeRatio(EAttributeType Type);
	void SetCurrentAttributeValue(EAttributeType Type, float Value);
	void SetCurrentAttributeValueToInt32(EAttributeType Type, int32 Value);
	void AddBonusAttribute(EAttributeType Type, float Value);

	FOnProgressBarDelegate OnProgressBarChanged;
	FOnHPIsZeroDelegate OnHPIsZero;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

private:
	FArcCharacterData* CurrentStatData;
	FTimerHandle ManaRegenerationTimerHandle;
	FTimerDelegate ManaRegenerationTimerDelegate;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 CurrentEXP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentMP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentATK;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentDEF;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float IncreasedManaRegeneration;

	float BonusATK = 0.f;
	float BonusDEF = 0.f;
	float BonusMaxHP = 0.f;
	float BonusMaxMP = 0.f;

	UFUNCTION(meta = (AllowPrivateAccess = true))
	void SetCurrentLevel(int32 NewLevel);

	UFUNCTION(meta = (AllowPrivateAccess = true))
	void ManaRegenerationPerSecond(float Amount);
};
