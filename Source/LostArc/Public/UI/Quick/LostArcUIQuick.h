// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LostArcUIQuick.generated.h"

UCLASS()
class LOSTARC_API ULostArcUIQuick : public UUserWidget
{
	GENERATED_BODY()

public:
	void ClearQuickSlot(int32 SlotIndex);
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY()
	TArray<class ULostArcUIQuickSlot*> QuickSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIQuickSlot* BP_EvadeSlot;
};
