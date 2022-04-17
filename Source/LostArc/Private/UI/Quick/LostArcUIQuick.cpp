// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Quick/LostArcUIQuick.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "UI/Quick/LostArcUIQuickSlot.h"
#include "Component/LostArcQuickSlotComponent.h"

void ULostArcUIQuick::NativeConstruct()
{
	Super::NativeConstruct();
	auto OwingChar = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn());

	for (int i = 0; i < 16; i++)
	{
		QuickSlot.Add(Cast<ULostArcUIQuickSlot>(GetWidgetFromName(FName(FString::Printf(TEXT("BP_QuickSlot_%d"), i)))));
		QuickSlot[i]->SetSlotComponent(OwingChar->QuickSlotComponent);
		QuickSlot[i]->SetSlotIndex(i);
		
		if(i < 8)
		{
			QuickSlot[i]->SetQuickSlotType(EQuickSlotType::Ability);
			QuickSlot[i]->SetAbilitySlot(static_cast<EAbilityType>(i+1), OwingChar->AbilityComponent);
		}
		else
		{
			QuickSlot[i]->SetQuickSlotType(EQuickSlotType::Potion);
		}
	}

	BP_EvadeSlot->SetAbilitySlot(EAbilityType::Evade, OwingChar->AbilityComponent);
}

void ULostArcUIQuick::ClearQuickSlot(int32 SlotIndex)
{
	QuickSlot[SlotIndex]->ClearSlotData();
}
