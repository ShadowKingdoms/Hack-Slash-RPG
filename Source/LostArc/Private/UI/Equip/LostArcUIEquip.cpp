// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Equip/LostArcUIEquip.h"
#include "Component/LostArcCharacterEquipComponent.h"
#include "UI/Equip/LostArcUIEquipSlot.h"
#include "UI/LostArcUITabs.h"

void ULostArcUIEquip::NativeConstruct()
{
	Super::NativeConstruct();
	OwnerCharacter = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn());

	for (int i = 0; i < 5; i++)
	{
		EquipSlot.Add(Cast<ULostArcUIEquipSlot>(GetWidgetFromName(FName(FString::Printf(TEXT("BP_EquipSlot_%d"), i))))); 
		EquipSlot[i]->SetSlotIndex(i);
	}

	OwnerCharacter->EquipComponent->EquipSlotUpdate.AddUObject(this, &ULostArcUIEquip::RefreshSlot);
	TitleBorder->SetUITabsFromParent(this);
}

void ULostArcUIEquip::BeginDestroy()
{
	Super::BeginDestroy();
	EquipSlot.Empty();
}

void ULostArcUIEquip::RefreshSlot(int32 Index)
{
	EquipSlot[Index]->RefreshSlotData(Cast<ULostArcAbilityBase>(OwnerCharacter->EquipComponent->GetAbility(Index)));
}