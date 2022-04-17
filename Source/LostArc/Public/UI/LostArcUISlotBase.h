// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Abilities/LostArcAbilityBase.h"
#include "UI/LostArcUISlotDrag.h"
#include "LostArcUISlotBase.generated.h"

enum ESlotType;

UCLASS()
class LOSTARC_API ULostArcUISlotBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetNativeTick(bool bCD);
	virtual void RefreshSlotData(class ULostArcAbilityBase* NewData);
	virtual void UnBindSlotData();
	void SetSlotIndex(int32 Index) { SlotIndex = Index; }
	void SetSlotComponent(UActorComponent* Compo){ SlotComponent = Compo; }
	
	void SetSlotType(ESlotType Type) { SlotType = Type; }
	UActorComponent* GetSlotComponent() {return SlotComponent;}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* Image_Icon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* Image_CD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UTextBlock* Text_CD;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DragVisualClass;

	UPROPERTY()
	class ULostArcAbilityBase* SlotData;

	UPROPERTY()
	UActorComponent* SlotComponent;
	
	ESlotType SlotType;
	int32 SlotIndex;
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	FDelegateHandle AbilityCDHandle;
	bool bEnableTick;
	
};
