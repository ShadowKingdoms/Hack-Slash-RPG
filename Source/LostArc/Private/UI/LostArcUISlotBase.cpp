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
				QuickSlotDragEffect();
				reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton); // 드래그 시작
			}
		}
	}
	
	return reply.NativeReply;
}

void ULostArcUISlotBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	// 드래그 이미지 크기 조절
	if (OutOperation == nullptr)
	{
		UUserWidget* DraggedVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualClass);
		UImage* DragIcon = Cast<UImage>(DraggedVisual->GetWidgetFromName("Image_Icon"));
		UImage* DragBorder = Cast<UImage>(DraggedVisual->GetWidgetFromName("Image_Border"));
		UTextBlock* DragTextBlock = Cast<UTextBlock>(DraggedVisual->GetWidgetFromName("Text_Quantity"));
		const auto DraggedItemObject = Cast<ULostArcItemBase>(SlotData);

		if(DragBorder != nullptr && Cast<UImage>(GetWidgetFromName("Image_BG")))
		{
			DragBorder->SetBrush(Cast<UImage>(GetWidgetFromName("Image_BG"))->Brush); // ..fromTexture는 true로 해야지 원본 텍스쳐 크기를 가져온다. (false는 0,0으로 설정됨)
			DragBorder->Brush.SetImageSize(FVector2D(112.f,112.f)); // 이미지 크기는 반드시 설정해준다.
		}

		else
		{
			DragBorder->SetVisibility(ESlateVisibility::Hidden);
		}
		

		if (DragIcon != nullptr)
		{
			DragIcon->SetBrush(Image_Icon->Brush); // ..fromTexture는 true로 해야지 원본 텍스쳐 크기를 가져온다. (false는 0,0으로 설정됨)
			DragBorder->Brush.SetImageSize(FVector2D(112.f,112.f));
		}
		
		if(DraggedItemObject->IsConsumable())
		{
			DragTextBlock->SetText(Cast<UTextBlock>(GetWidgetFromName("Text_Quantity"))->GetText());
		}
		else
		{
			DragTextBlock->SetVisibility(ESlateVisibility::Hidden);
		}
		
		// 드래그 시작 위치의 슬롯 정보를 저장 (OnDrop에서 Owner가 됨)
		ULostArcUISlotDrag* oper = NewObject<ULostArcUISlotDrag>();
		oper->DefaultDragVisual = DraggedVisual;
		oper->SlotComponent = this->SlotComponent; // 드래그 시 이미 현재 슬롯 컴포넌트를 도착지 컴포넌트로 교체함
		oper->SlotIndex = this->SlotIndex;
		oper->SlotType = this->SlotType;
		OutOperation = oper;

		SlotDragEffect();
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
		SlotDropEffect();
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