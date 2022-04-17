// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LostArcUISlotDrag.h"
#include "Blueprint/UserWidget.h"
#include "Equip/LostArcUIEquip.h"
#include "Inventory/LostArcUIInven.h"
#include "Quick/LostArcUIQuick.h"
#include "LostArcUIMainHUD.generated.h"

UCLASS()
class LOSTARC_API ULostArcUIMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIProgressBar* BP_HPBAR;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIProgressBar* BP_MPBAR;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIProgressBar* BP_EXPBAR;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIQuick* BP_Quick;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIInven* BP_Inven;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class ULostArcUIEquip* BP_Equip;

	void EnableWidget(ESlotType EType);

public:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
