#include "Component/LostArcQuickSlotComponent.h"
#include "Abilities/Items/LostArcItemBase.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "Component/LostArcInventoryComponent.h"

ULostArcQuickSlotComponent::ULostArcQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	QuickSlot.SetNum(16);
}

void ULostArcQuickSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void ULostArcQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();
}

ULostArcAbilityBase* ULostArcQuickSlotComponent::GetAbility(int32 SlotIndex, bool bTrans)
{
	if(bTrans)
	{
		auto TransUnit = QuickSlot[SlotIndex];
		QuickSlot[SlotIndex] = nullptr;
		return TransUnit;
	}
	else
	{
		return QuickSlot[SlotIndex];
	}
}

void ULostArcQuickSlotComponent::UseAbility(int32 SlotIndex)
{
	if(SlotIndex < 8) // Skills
	{
		Cast<ALostArcPlayerCharacter>(GetOwner())->AbilityComponent->AbilityCast(static_cast<EAbilityType>(SlotIndex+1));
	}

	else if (QuickSlot[SlotIndex] != nullptr)
	{
		if(QuickSlot[SlotIndex]->Use(Cast<ALostArcPlayerCharacter>(GetOwner())))
		{
			Cast<ALostArcPlayerCharacter>(GetOwner())->InventoryComponent->DeleteItemAfterCheckingQuantity(); // 매번 인벤토리 전체를 스캔해서 current가 0인 슬롯을 삭제함
		}
	}
}