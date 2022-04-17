	// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/LostArcUISlotBase.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "Component/LostArcInventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/LostArcAbilityInterface.h"
#include "Controller/LostArcPlayerController.h"
#include "UI/LostArcUIMainHUD.h"

void ULostArcUISlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	bEnableTick = false;
	SlotIndex = 0;
}

void ULostArcUISlotBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bEnableTick) 
	{
		Super::NativeTick(MyGeometry, InDeltaTime);
		auto Character = Cast<ALostArcPlayerCharacter>(GetOwningPlayerPawn());

		if (GetOwningPlayer()->GetWorldTimerManager().IsTimerActive(SlotData->AbilityCDProperty.Key))
		{
			float Value = GetOwningPlayer()->GetWorldTimerManager().GetTimerRemaining(SlotData->AbilityCDProperty.Key) / SlotData->GetCDTime();
			Image_CD->GetDynamicMaterial()->SetScalarParameterValue(FName(TEXT("Progress")), Value < 0.01f ? 0.0f : Value);
		}
	}
}

FReply ULostArcUISlotBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if(Cast<ULostArcSkillBase>(SlotData))
	{
		return reply.NativeReply;
	}
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) // Right Mouse Clicked
	{
		Cast<ALostArcPlayerController>(GetOwningPlayer())->bOnSlotClick = true;
		
		if (SlotData != nullptr)
		{
			auto Inter = Cast<ILostArcAbilityInterface>(SlotComponent);
			if (Inter != nullptr)
			{
				Inter->UseAbility(SlotIndex);
				return reply.NativeReply;
			}
		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) // Left Mouse Clicked
	{
		if (SlotData)
		{
			if (!GetOwningPlayer()->GetWorldTimerManager().IsTimerActive(SlotData->AbilityCDProperty.Key))
			{
				reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton); // 드래그 시작
			}
		}
	}
	
	return reply.NativeReply;
}

void ULostArcUISlotBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		ULostArcUISlotDrag* oper = NewObject<ULostArcUISlotDrag>();
		UUserWidget* DraggedItem = CreateWidget<UUserWidget>(GetWorld(), DragVisualClass);
		UImage* ImageBox = Cast<UImage>(DraggedItem->GetWidgetFromName("Image_Item"));

		if (ImageBox != nullptr)
		{
			ImageBox->SetBrushFromTexture(SlotData->GetAbility_Icon());
		}
		
		// 드래그 시작 위치의 슬롯 정보를 저장 (OnDrop에서 Owner가 됨)
		oper->DefaultDragVisual = DraggedItem;
		oper->SlotComponent = this->SlotComponent; // 드래그 시 이미 현재 슬롯 컴포넌트를 도착지 컴포넌트로 교체함
		oper->SlotIndex = this->SlotIndex;
		oper->SlotType = this->SlotType;
		OutOperation = oper;
	}
}

bool ULostArcUISlotBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	ULostArcUISlotDrag* Owner = Cast<ULostArcUISlotDrag>(InOperation);
		
	if(Owner->SlotType == ESlotType::TitleTab) // 위젯끼리 충돌시
	{
		ULostArcUIMainHUD* MainHUD = Cast<ALostArcPlayerController>(GetOwningPlayer())->MainHUD;
		MainHUD->NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
		return true;
	}
	
	if(this->SlotType == Owner->SlotType)
	{
		auto Interface = Cast<ILostArcAbilityInterface>(SlotComponent);
		if (Interface != nullptr)
		{
			Interface->SwappingSlot(Owner->SlotIndex, this->SlotIndex); // 내수용 스왑
		}
		return true;
	}
	
	return false;
}

void ULostArcUISlotBase::SetNativeTick(bool bCD)
{
	bEnableTick = bCD;

	if (bCD)
	{
		Image_CD->SetVisibility(ESlateVisibility::Visible);
		if (SlotType == ESlotType::Evadeing) SetVisibility(ESlateVisibility::Visible);
	}
	
	else
	{
		Image_CD->SetVisibility(ESlateVisibility::Hidden);
		if (SlotType == ESlotType::Evadeing) SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULostArcUISlotBase::RefreshSlotData(ULostArcAbilityBase* NewData)
{
	if (NewData == nullptr)
	{
		SlotData = nullptr;
		Image_Icon->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SlotData = NewData;
		AbilityCDHandle = SlotData->AbilityCDProperty.Value.AddUObject(this, &ULostArcUISlotBase::SetNativeTick);
		if (SlotData->GetAbility_Icon() != nullptr)
		{
			Image_Icon->SetBrushFromTexture(SlotData->GetAbility_Icon());
			Image_Icon->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ULostArcUISlotBase::UnBindSlotData()
{
	if (SlotData != nullptr)
	{
		SlotData->AbilityCDProperty.Value.Remove(AbilityCDHandle);
	}
}