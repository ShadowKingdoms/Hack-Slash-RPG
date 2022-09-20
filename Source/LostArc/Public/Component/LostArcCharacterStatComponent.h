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
	CRT UMETA(DisplayName = "CRT")
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
	float GetMaxAttributeValue(EAttributeType Type);
	float GetCurrentAttributeRatio(EAttributeType Type);


	
	UFUNCTION(BlueprintCallable)
	float GetCurrentAttributeValue(EAttributeType Type);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAttributeValue(EAttributeType Type, float Value);
	
	void AddBonusAttribute(EAttributeType Type, float Value);

	UFUNCTION(meta = (AllowPrivateAccess = true))
	void ManaRegenerationPerSecond(float Amount);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	float GetCriticalRate() { return GetCurrentAttributeValue(EAttributeType::CRT); }

	
	FOnProgressBarDelegate OnProgressBarChanged;
	FOnHPIsZeroDelegate OnHPIsZero;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

private:
	FArcCharacterStatData* CurrentStatData;
	FTimerHandle ManaRegenerationTimerHandle;
	FTimerDelegate ManaRegenerationTimerDelegate;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentMP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentATK;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentDEF;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentCritical;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float PlayerHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float PlayerMP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float IncreasedManaRegeneration;
	

	float BonusATK = 0.f;
	float BonusDEF = 0.f;
	float BonusMaxHP = 0.f;
	float BonusMaxMP = 0.f;
	float BonusCritical = 0.f;
};
