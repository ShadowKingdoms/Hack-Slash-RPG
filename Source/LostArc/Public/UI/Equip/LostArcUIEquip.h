// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LostArcUIEquip.generated.h"

enum EAttributeType;

UCLASS()
class LOSTARC_API ULostArcUIEquip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class ULostArcUITabs* TitleBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* TextBlock_MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* TextBlock_MaxMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* TextBlock_Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* TextBlock_Defense;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* TextBlock_Critical;
	
	void RefreshSlot(int32 Index);
	void UpdateCurrentStatText(EAttributeType Type);

protected:
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;
	
private:
	UPROPERTY()
	TArray<class ULostArcUISlotBase*> EquipSlot;

	UPROPERTY()
	class ALostArcPlayerCharacter* OwnerCharacter;
};
