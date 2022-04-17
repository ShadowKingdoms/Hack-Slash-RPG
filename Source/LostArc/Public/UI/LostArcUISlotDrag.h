// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LostArcUITabs.h"
#include "Blueprint/DragDropOperation.h"
#include "Abilities/Items/LostArcItemBase.h"
#include "LostArcUISlotDrag.generated.h"

UENUM(BlueprintType)
enum ESlotType
{
	Inven UMETA(DisplayName = "Inven"),
	Equip UMETA(DisplayName = "Equip"),
	Skill UMETA(DisplayName = "Skill"),
	Quick UMETA(DisplayName = "Quick"),
	Evadeing UMETA(DisplayName = "Evade"),
	TitleTab UMETA(DisplayName = "TitleTab")
};

UCLASS()
class LOSTARC_API ULostArcUISlotDrag : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	TEnumAsByte<ESlotType> SlotType;
	
	UActorComponent* SlotComponent;
	int32 SlotIndex;
	
	UUserWidget* WidgetToDrag;
	FVector2D MouseOffset;
};
