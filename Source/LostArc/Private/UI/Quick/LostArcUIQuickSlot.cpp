// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Quick/LostArcUIQuickSlot.h"
#include "Abilities/Items/Equip/LostArcItemEquipBase.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/LostArcAbilityInterface.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "Component/LostArcQuickSlotComponent.h"
#include "Controller/LostArcPlayerController.h"
#include "UI/LostArcUIMainHUD.h"

void ULostArcUIQuickSlot::SetNativeTick(bool bCD)
{
	bEnableTick = bCD;
	
	if(QuickSlotType == EQuickSlotType::Ability)
	{
		bCD ? Text_CD->SetVisibility(ESlateVisibility::Visible) : Text_CD->SetVisibility(ESlateVisibility::Hidden);
	}

	Super::SetNativeTick(bCD);
}

void ULostArcUIQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	SlotType = ESlotType::Quick;
	Text_Key->SetText(KeyName);
	Image_Icon->SetBrushFromTexture(ThumbTexture2D); // 텍스트와 이미지를 블루프린트에서 가져옴
}

bool ULostArcUIQuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	auto OwnerDrag = Cast<ULostArcUISlotDrag>(InOperation);
	if(SlotData && GetOwningPlayer()->GetWorldTimerManager().IsTimerActive(SlotData->AbilityCDProperty.Key)) return false;
	if(OwnerDrag->SlotType == ESlotType::TitleTab) return false;

	switch (QuickSlotType)
	{
		case EQuickSlotType::Ability:
			break;

		case EQuickSlotType::Potion:
			ILostArcAbilityInterface * Interface = Cast<ILostArcAbilityInterface>(OwnerDrag->SlotComponent);
			if(Cast<ULostArcItemEquipBase>(Interface->GetAbility(OwnerDrag->SlotIndex))) return false;
		
			if(OwnerDrag->SlotType == ESlotType::Inven)
			{
				auto OwnerItem = Cast<ULostArcItemBase>(Interface->GetAbility(OwnerDrag->SlotIndex));
				RefreshSlotData(OwnerItem);
			
				RefIndex = OwnerDrag->SlotIndex;
				ItemQuantityHandle = OwnerItem->QuantityUpdate.AddUObject(this, &ULostArcUIQuickSlot::UpdateQuantity);
				Text_Quantity->SetText(FText::AsNumber(FMath::FloorToInt(OwnerItem->GetItemQuantity())));
				Text_Quantity->SetVisibility(ESlateVisibility::Visible);
				Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn())->QuickSlotComponent->QuickSlot[SlotIndex] = Interface->GetAbility(OwnerDrag->SlotIndex); // 컴포넌트에 데이터 복사
			}
		
			if(OwnerDrag->SlotType == ESlotType::Quick) // 위젯 계층은 하위 위젯에서 상위 위젯(MainHUD)으로 올라가는 방식이다. (false을 반환할 시 MainHUD의 OnDrop을 실행)
			{
				if(OwnerDrag->SlotIndex == SlotIndex) return true;
			
				auto OwnerItem = Cast<ULostArcItemBase>(Interface->GetAbility(OwnerDrag->SlotIndex,true));
				Cast<ALostArcPlayerController>(GetOwningPlayer())->MainHUD->BP_Quick->ClearQuickSlot(OwnerDrag->SlotIndex);
				RefreshSlotData(OwnerItem);
			
				RefIndex = OwnerDrag->SlotIndex;
				ItemQuantityHandle = OwnerItem->QuantityUpdate.AddUObject(this, &ULostArcUIQuickSlot::UpdateQuantity);
				Text_Quantity->SetText(FText::AsNumber(FMath::FloorToInt(OwnerItem->GetItemQuantity())));
				Text_Quantity->SetVisibility(ESlateVisibility::Visible);
				Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn())->QuickSlotComponent->QuickSlot[SlotIndex] = OwnerItem;
			}
		
			return true;
	}
	
	return false;
}

void ULostArcUIQuickSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	auto OwnerDrag = Cast<ULostArcUISlotDrag>(InOperation);
	ILostArcAbilityInterface * Interface = Cast<ILostArcAbilityInterface>(OwnerDrag->SlotComponent);
	
	Cast<ULostArcItemBase>(Interface->GetAbility(OwnerDrag->SlotIndex,true));
	Cast<ALostArcPlayerController>(GetOwningPlayer())->MainHUD->BP_Quick->ClearQuickSlot(OwnerDrag->SlotIndex);
}

void ULostArcUIQuickSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(QuickSlotType == EQuickSlotType::Ability)
	{
		if(bEnableTick)
		{
			auto Character = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn());
			float Count = GetOwningPlayer()->GetWorldTimerManager().GetTimerRemaining(Character->AbilityComponent->GetAbilities(static_cast<EAbilityType>(SlotIndex+1))->AbilityCDProperty.Key);
	
			Text_CD->SetText(FText::AsNumber(FMath::FloorToInt(Count)));
		}
	}
}

FReply ULostArcUIQuickSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void ULostArcUIQuickSlot::RefreshSlotData(ULostArcAbilityBase* NewData)
{
	if(SlotData != nullptr)
	{
		UnBindSlotData();
	}
	
	Super::RefreshSlotData(NewData);
	
	UpdateQuantity();
}

void ULostArcUIQuickSlot::UnBindSlotData()
{
	Super::UnBindSlotData();
	
	auto SlotItem = Cast<ULostArcItemBase>(SlotData);
	if (SlotItem != nullptr)
	{
		SlotItem->QuantityUpdate.Remove(ItemQuantityHandle);
	}
}

void ULostArcUIQuickSlot::UpdateQuantity()
{
	if (SlotData != nullptr) 
	{
		if(FMath::FloorToInt(Cast<ULostArcItemBase>(SlotData)->GetItemQuantity()) <=0) // 삭제해도 계속 남아있음(수정필요)
		{
			Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn())->QuickSlotComponent->QuickSlot[SlotIndex] = nullptr;
			// 인벤토리 슬롯의 인덱스 추적은 불가
			Text_Quantity->SetVisibility(ESlateVisibility::Hidden);
			Image_Icon->SetVisibility(ESlateVisibility::Hidden);
			UnBindSlotData();
			SlotData = nullptr;
		}
		else
		{
			Text_Quantity->SetText(FText::AsNumber(FMath::FloorToInt(Cast<ULostArcItemBase>(SlotData)->GetItemQuantity())));
		}
	}
}

void ULostArcUIQuickSlot::ClearSlotData()
{
	Text_Quantity->SetVisibility(ESlateVisibility::Hidden);
	Image_Icon->SetVisibility(ESlateVisibility::Hidden);
	Image_CD->SetVisibility(ESlateVisibility::Hidden);

	SlotData->AbilityCDProperty.Value.Remove(AbilityCDHandle); // 엔진에 등록된 AbilityCDHandle 델리게이트 핸들을 삭제
	SlotData = nullptr;
}

void ULostArcUIQuickSlot::SetAbilitySlot(EAbilityType EType, ULostArcCharacterAbilityComponent* Compo)
{
	SlotData = Compo->GetAbilities(EType);
	Image_Icon->SetVisibility(ESlateVisibility::Visible);
	
	Compo->GetAbilities(EType)->AbilityCDProperty.Value.AddUObject(this, &ULostArcUIQuickSlot::SetNativeTick);
	
	if(EType == EAbilityType::Evade)
	{
		Image_Tri->SetVisibility(ESlateVisibility::Hidden);
		SetSlotIndex(static_cast<int32>(EAbilityType::Evade)-1); // NativeTick에서 +1 해주니 미리 SlotIndex을 -1 해준다.
		SetQuickSlotType(EQuickSlotType::Ability);
		SetVisibility(ESlateVisibility::Hidden);
		SetSlotType(ESlotType::Evadeing);
	}
}