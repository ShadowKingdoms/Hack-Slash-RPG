// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/LostArcAbilityBase.h"
#include "LostArcItemBase.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnItemQuantityUpdateDelegate);

UENUM(BlueprintType)
enum EItemType
{
	ITEM_Equip UMETA(DisplayName = "Equip"),
	ITEM_Potion UMETA(DisplayName = "Potion"),
	ITEM_None  UMETA(DisplayName = "None")
};

UCLASS(BlueprintType)
class LOSTARC_API ULostArcItemBase : public ULostArcAbilityBase
{
	GENERATED_BODY()
	
public:
	UTexture2D* BgTexture2D;
	FOnItemQuantityUpdateDelegate QuantityUpdate;
	
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
	virtual bool AbilityStateCheck(ALostArcPlayerCharacter* Character) override;
	bool IsConsumable() const;
	virtual void ItemConstruct();
	bool SetItemQuantity(int32 Count);
	int32 GetItemQuantity() { return ItemQuantity; }
	UTexture2D* GetBgTexture2D() { return BgTexture2D; }
	EItemType GetItemType() { return ItemType; }
	
	UFUNCTION(BlueprintCallable)
	float GetItemValue() const {return Value;}

	UFUNCTION(BlueprintCallable)
	EAttributeType GetAtbType() const { return AtbType; }

protected:
	int32 MaxCount;
	int32 ItemQuantity;
	EItemType ItemType;
	float Value = 0.f;
	EAttributeType AtbType;
};
