// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/LostArcUIInven.h"
#include "UI/Inventory/LostArcUIInvenSlot.h"
#include "Component/LostArcInventoryComponent.h"
#include "UI/LostArcUITabs.h"

void ULostArcUIInven::NativeConstruct()
{
	Super::NativeConstruct();
	OwnerCharacter = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn());

	for (int i = 0; i < 16; i++)
	{
		InvenSlot.Add(Cast<ULostArcUIInvenSlot>(GetWidgetFromName(FName(FString::Printf(TEXT("BP_InvenSlot_%d"), i))))); 
		InvenSlot[i]->SetSlotIndex(i);
	}

	OwnerCharacter->InventoryComponent->InvenSlotUpdate.AddUObject(this, &ULostArcUIInven::RefreshSlot);
	TitleBorder->SetUITabsFromParent(this);
}

void ULostArcUIInven::BeginDestroy()
{
	Super::BeginDestroy();
	InvenSlot.Empty();
}

void ULostArcUIInven::RefreshSlot(int32 Index)
{
	InvenSlot[Index]->RefreshSlotData(Cast<ULostArcAbilityBase>(OwnerCharacter->InventoryComponent->GetAbility(Index)));
}