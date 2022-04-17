// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/LostArcUIInvenSlot.h"

#include "Abilities/Items/Equip/LostArcItemEquipBase.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "Component/LostArcAbilityInterface.h"
#include "Component/LostArcInventoryComponent.h"
#include "Controller/LostArcPlayerController.h"

void ULostArcUIInvenSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SlotType = ESlotType::Inven;
	SlotComponent = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn())->InventoryComponent;
}

bool ULostArcUIInvenSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	ULostArcUISlotDrag* Owner = Cast<ULostArcUISlotDrag>(InOperation);
	
	if(Owner->SlotType == ESlotType::Quick) return false;
	if(!Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation))
	{
		auto Interface = Cast<ILostArcAbilityInterface>(SlotComponent);
		
		if (Interface != nullptr)
		{
			Interface->SwappingSlot(Owner->SlotIndex, this->SlotIndex, Owner->SlotComponent);
			return true;
		}
	}

	return false;
}

FReply ULostArcUIInvenSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(Cast<ULostArcSkillBase>(SlotData))
		return Super::Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) // Right Mouse Clicked
	{
		Cast<ALostArcPlayerController>(GetOwningPlayer())->bOnSlotClick = true;
		auto APlayer = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn());
		auto EquipData = Cast<ULostArcItemEquipBase>(SlotData);
		if(EquipData != nullptr) // 장신구 아이템을 장착할 때
		{
			if(APlayer->InventoryComponent->InventorySlotEmptyCheck()) // 인벤토리가 한 칸 이상 비었을 경우에는
			{
				return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent); // 아무 곳이나 장착 가능
			}
			else // 인벤토리가 꽉 찼을 경우엔
			{
				if(APlayer->EquipComponent->EquipSlotEmptyCheck(EquipData)) // 해당 장비 슬롯이 하나 이상 비었을 때만 장착 가능
				{
					return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
				}
			}
		}
		else
		{
			return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) // LeftMouseClick
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}
		
	return Super::Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
}

void ULostArcUIInvenSlot::RefreshSlotData(ULostArcAbilityBase* NewData)
{
	if (SlotData != nullptr) // Unbinding Delegate
	{
		UnBindSlotData();		
	}

	Super::RefreshSlotData(NewData);
	
	if (NewData != nullptr)
	{
		auto SlotItem = dynamic_cast<ULostArcItemBase*>(SlotData);
		if (SlotItem == nullptr) return;
		
		if (SlotItem->GetBgTexture2D() != nullptr)
		{
			Image_BG->SetBrushFromTexture(SlotItem->GetBgTexture2D());
			Image_BG->SetVisibility(ESlateVisibility::Visible);
		}
		if (SlotItem->IsConsumable())
		{
			UpdateQuantity();
			Text_Quantity->SetVisibility(ESlateVisibility::Visible);
		}
		ItemQuantityHandle = SlotItem->QuantityUpdate.AddUObject(this, &ULostArcUIInvenSlot::UpdateQuantity);
	}
}

void ULostArcUIInvenSlot::UnBindSlotData()
{
	Super::UnBindSlotData();
	
	Image_BG->SetVisibility(ESlateVisibility::Hidden);
	Text_Quantity->SetVisibility(ESlateVisibility::Hidden);

	auto SlotItem = dynamic_cast<ULostArcItemBase*>(SlotData);
	if (SlotItem != nullptr)
	{
		SlotItem->QuantityUpdate.Remove(ItemQuantityHandle);
	}
}

void ULostArcUIInvenSlot::UpdateQuantity()
{
	if (SlotData != nullptr) 
	{
		Text_Quantity->SetText(FText::AsNumber(FMath::FloorToInt(dynamic_cast<ULostArcItemBase*>(SlotData)->GetItemQuantity())));
	}
}