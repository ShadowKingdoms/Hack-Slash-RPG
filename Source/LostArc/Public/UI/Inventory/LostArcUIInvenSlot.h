// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LostArcUISlotBase.h"
#include "LostArcUIInvenSlot.generated.h"

UCLASS()
class LOSTARC_API ULostArcUIInvenSlot : public ULostArcUISlotBase
{
	GENERATED_BODY()
	
public:
	virtual void RefreshSlotData(class ULostArcAbilityBase* NewData) override;
	virtual void UnBindSlotData() override;
	void UpdateQuantity();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* Image_BG;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* Text_Quantity;

	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
private:
	FDelegateHandle ItemQuantityHandle;
};