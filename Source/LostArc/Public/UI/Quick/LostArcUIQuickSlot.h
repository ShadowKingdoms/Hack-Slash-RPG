// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LostArcUISlotBase.h"
#include "LostArcUIQuickSlot.generated.h"

UENUM(BlueprintType)
enum EQuickSlotType
{
	Ability UMETA(DisplayName = "Ability"),
	Potion UMETA(DisplayName = "Potion"),
};

UCLASS()
class LOSTARC_API ULostArcUIQuickSlot : public ULostArcUISlotBase
{
	GENERATED_BODY()

public:
	virtual void SetNativeTick(bool CD) override;
	void SetAbilitySlot(EAbilityType EType, class ULostArcCharacterAbilityComponent* Compo);
	void SetQuickSlotType(EQuickSlotType Type) { QuickSlotType = Type; }
	virtual void RefreshSlotData(ULostArcAbilityBase* NewData) override;
	virtual void UnBindSlotData() override;
	void UpdateQuantity();
	void ClearSlotData();
	
protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* Image_Tri;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* Text_Key;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* Text_Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = true))
	class UTexture2D * ThumbTexture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = true))
	FText KeyName;
	
	int32 RefIndex;
	EQuickSlotType QuickSlotType;
	FDelegateHandle ItemQuantityHandle;
};
