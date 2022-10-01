// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LostArcInventoryComponent.h"
#include "Abilities/Items/LostArcItemBase.h"
#include "Component/LostArcAbilityInterface.h"
#include "Controller/LostArcPlayerController.h"
#include "UI/LostArcUIMainHUD.h"

ULostArcInventoryComponent::ULostArcInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	ItemClass.Init(ULostArcItemBase::StaticClass(), 5);
}
void ULostArcInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (int i = 0; i < 5; i++)
	{
		ItemTable.Add(ItemClass[i].GetDefaultObject()->GetName(), ItemClass[i]); // 아이템 테이블에 모든 아이템 정보를 넣는다.
	}
	
	InventorySlot.SetNum(16); // InvenSlot을 Null로 초기화 
}
void ULostArcInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
void ULostArcInventoryComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	InventorySlot.Empty();
	ItemTable.Empty();
}

void ULostArcInventoryComponent::UseAbility(int32 SlotIndex)
{
	if (InventorySlot[SlotIndex] != nullptr)
	{
		if (InventorySlot[SlotIndex]->Use(Cast<ALostArcPlayerCharacter>(GetOwner()))) // 아이템을 모두 소모했을 경우
		{
			InventorySlot[SlotIndex] = nullptr;
			InvenSlotUpdate.Broadcast(SlotIndex);
		}
	}
}
void ULostArcInventoryComponent::SwappingSlot(int32 OwnerIndex, int32 DistIndex, UActorComponent* OwnerComponent)
{
	if(OwnerComponent == nullptr) // 내수 교환
	{
		if(GetAbility(OwnerIndex) == nullptr || OwnerIndex == DistIndex) return;

		if (GetAbility(DistIndex) == nullptr)
		{
			SetAbility(GetAbility(OwnerIndex,true), DistIndex);
		}
		else
		{
			if (!GetOwner()->GetWorldTimerManager().IsTimerActive(InventorySlot[DistIndex]->AbilityCDProperty.Key))
			{
				if(Cast<ULostArcItemBase>(GetAbility(OwnerIndex))->IsConsumable() && Cast<ULostArcItemBase>(GetAbility(DistIndex))->IsConsumable()) // 같은 소비 아이템일 경우
				{
					if(GetAbility(OwnerIndex)->GetName() == GetAbility(DistIndex)->GetName())
					{
						InventorySlot[DistIndex]->SetItemQuantity(Cast<ULostArcItemBase>(GetAbility(OwnerIndex,true))->GetItemQuantity());
						return;
					}
				}
				Swap(InventorySlot[OwnerIndex], InventorySlot[DistIndex]);
				InvenSlotUpdate.Broadcast(OwnerIndex);
				InvenSlotUpdate.Broadcast(DistIndex);
			}
		}
	}
	else // 외부 슬롯 교환
	{
		ILostArcAbilityInterface* Interface = Cast<ILostArcAbilityInterface>(OwnerComponent);
		if(Interface == nullptr) return;
		
		auto OwnerItem = Cast<ULostArcItemBase>(Interface->GetAbility(OwnerIndex));
		if(OwnerItem == nullptr) return;
		
		if(GetAbility(DistIndex) == nullptr) // Drop 위치의 슬롯이 null일 때
		{
			SetAbility(Interface->GetAbility(OwnerIndex, true), DistIndex);
		}
		else
		{	// Owner와 this가 소비형 아이템(IsConsumable)일 경우 어빌리티 이름(Name)을 체크하여 같으면 수량 증가
			auto OwnerData = Cast<ULostArcItemBase>(Interface->GetAbility(OwnerIndex));
			if(OwnerData == nullptr) return;

			if(Cast<ULostArcItemBase>(GetAbility(DistIndex))->IsConsumable() && OwnerData->IsConsumable()) // 같은 소비 아이템일 경우
			{
				if(OwnerData->GetName() == GetAbility(DistIndex)->GetName())
				{
					InventorySlot[DistIndex]->SetItemQuantity(Cast<ULostArcItemBase>(GetAbility(OwnerIndex,true))->GetItemQuantity());
				}
			}
			else if(Cast<ULostArcItemBase>(GetAbility(DistIndex))->GetItemType() == OwnerData->GetItemType()) // 장비 아이템일 경우
			{
				Interface->SwappingSlot(DistIndex, OwnerIndex, this);
			}
		}
	}
	
}
bool ULostArcInventoryComponent::SetAbility(ULostArcAbilityBase* OwnerAbility, int32 SlotIndex)
{
	auto OwnerItem = Cast<ULostArcItemBase>(OwnerAbility);
	if(OwnerItem == nullptr) return false;
	
	if(SlotIndex == -1) // one click
	{
		for(int i = 0; i < 16; i++)
		{
			if(InventorySlot[i] == nullptr)
			{
				InventorySlot[i] = OwnerItem;
				InvenSlotUpdate.Broadcast(i);
				return true;
			}
		}
		
		return false;
	}
	else // drag
	{
		if(InventorySlot[SlotIndex] == nullptr)
		{
			InventorySlot[SlotIndex] = OwnerItem;
			InvenSlotUpdate.Broadcast(SlotIndex);
			return true;
		}
		
		return false;
	}
}
ULostArcAbilityBase* ULostArcInventoryComponent::GetAbility(int32 SlotIndex, bool bTrans)
{
	if(bTrans)
	{
		auto TransUnit = InventorySlot[SlotIndex];

		if(TransUnit != nullptr)
		{
			InventorySlot[SlotIndex] = nullptr;
			InvenSlotUpdate.Broadcast(SlotIndex);
			return TransUnit;
		}
	
		return nullptr;
	}
	else
	{
		if (InventorySlot[SlotIndex] == nullptr)
			return nullptr;
		else
			return InventorySlot[SlotIndex];
	}
}

bool ULostArcInventoryComponent::InventorySlotEmptyCheck()
{
	int32 iCurrentSlotCount = 0;
	
	for (int i = 0; i < MaxSlotCount; i++) 
	{
		if (InventorySlot[i] != nullptr)
		{
			iCurrentSlotCount++;
		}
	}
	return iCurrentSlotCount == MaxSlotCount ? false : true;
}

void ULostArcInventoryComponent::AddPickupItem(FString ItemName, int32 ItemCount)
{
	if (ItemTable.Find(ItemName) == nullptr) return;
	const auto NewItem = ItemTable.Find(ItemName)->GetDefaultObject();
	const auto Player = Cast<ALostArcPlayerCharacter>(GetOwner());
	const auto PController = Cast<ALostArcPlayerController>(Player->GetController());

	if (NewItem)
	{
		if (NewItem->IsConsumable()) // 소비 아이템
		{
			for (int i = 0; i < MaxSlotCount; i++) // 중복
			{
				if (InventorySlot[i] != nullptr)
				{
					if (InventorySlot[i]->GetName() == NewItem->GetName()) // 인벤에 이미 있으면
					{
						InventorySlot[i]->SetItemQuantity(ItemCount); // 수량만 증가

						const auto StringName = InventorySlot[i]->GetName();
						const auto TextureBg = InventorySlot[i]->GetBgTexture2D();
						const auto TextureIcon = InventorySlot[i]->GetAbility_Icon();
						PController->MainHUD->CallInventoryItemNotice(TextureBg, TextureIcon, FText::FromString(StringName));
						
						return;
					}
				}
			}
			for (int i = 0; i < MaxSlotCount; i++) // 새로운
			{
				if (InventorySlot[i] == nullptr)
				{
					InventorySlot[i] = NewObject<ULostArcItemBase>(this, ItemTable.Find(ItemName)->Get()); // 새로운 아이템을 인벤에 추가
					InventorySlot[i]->SetItemQuantity(ItemCount); // 수량 증가
					InvenSlotUpdate.Broadcast(i);

					const auto StringName = InventorySlot[i]->GetName();
					const auto TextureBg = InventorySlot[i]->GetBgTexture2D();
					const auto TextureIcon = InventorySlot[i]->GetAbility_Icon();
					PController->MainHUD->CallInventoryItemNotice(TextureBg, TextureIcon, FText::FromString(StringName));
					
					return;
				}
			}
		}
		else // 장비 아이템
		{
			for (int i = 0; i < MaxSlotCount; i++)
			{
				if (InventorySlot[i] == nullptr)
				{
					InventorySlot[i] = NewObject<ULostArcItemBase>(this, ItemTable.Find(ItemName)->Get());
					InvenSlotUpdate.Broadcast(i);

					const auto StringName = InventorySlot[i]->GetName();
					const auto TextureBg = InventorySlot[i]->GetBgTexture2D();
					const auto TextureIcon = InventorySlot[i]->GetAbility_Icon();
					PController->MainHUD->CallInventoryItemNotice(TextureBg, TextureIcon, FText::FromString(StringName));
					
					return;
				}
			}
		}
	}
}

void ULostArcInventoryComponent::DeleteItemAfterCheckingQuantity() // 포션 아이템의 수량을 확인하고 0인 아이템이 슬롯에 있으면 삭제한다.
{
	for(int i = 0; i < 16; i++)
	{
		if(InventorySlot[i] != nullptr)
		{
			if(InventorySlot[i]->GetItemType() == EItemType::ITEM_Potion)
			{
				if(InventorySlot[i]->GetItemQuantity() <= 0)
				{
					InventorySlot[i] = nullptr;
					InvenSlotUpdate.Broadcast(i);
					break;
				}
			}
		}
	}
}
