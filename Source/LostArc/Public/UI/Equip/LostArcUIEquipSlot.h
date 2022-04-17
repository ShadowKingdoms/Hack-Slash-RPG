// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LostArcUISlotBase.h"
#include "LostArcUIEquipSlot.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcUIEquipSlot : public ULostArcUISlotBase
{
	GENERATED_BODY()
	
public:
	virtual void RefreshSlotData(class ULostArcAbilityBase* NewData) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* Image_BG;
	
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override; 
};
