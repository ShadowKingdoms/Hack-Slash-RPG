// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LostArcCharacterEquipComponent.h"
#include "Abilities/Items/LostArcItemBase.h"
#include "Abilities/Items/Equip/LostArcItemEquipBase.h"
#include "Component/LostArcAbilityInterface.h"

ULostArcCharacterEquipComponent::ULostArcCharacterEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	for (int i = 0; i < 3; i++)
	{
		EquipSlot.Add((EAccessoryType)i);
		EquipMaxSlot.Add((EAccessoryType)i);
	}
}
void ULostArcCharacterEquipComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	for (int32 i = 0; i < 3; i++)
	{
		if (EquipMaxSlot.Find((EAccessoryType)i) != nullptr)
		{
			EquipSlot.Find((EAccessoryType)i)->EquipArray.SetNum(*EquipMaxSlot.Find((EAccessoryType)i));
		}
	}

	Interface = Cast<ALostArcPlayerCharacter>(GetOwner())->InventoryComponent;
}
void ULostArcCharacterEquipComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EquipSlot.Empty();
}

void ULostArcCharacterEquipComponent::UseAbility(int32 SlotIndex)
{
	auto Equip = Cast<ULostArcItemEquipBase>(GetAbility(SlotIndex, true));
	
	if(Equip != nullptr)
	{
		Interface->SetAbility(Equip);
	}
}
void ULostArcCharacterEquipComponent::SwappingSlot(int32 OwnerIndex, int32 DistIndex, UActorComponent* OwnerComponent)
{
	if(OwnerComponent == nullptr) // 내부 슬롯 스왑
	{
		if(GetAbility(OwnerIndex) == nullptr || OwnerIndex == DistIndex) return;
		
		if (GetAbility(DistIndex) == nullptr) // 이동
		{
			auto OwnerEquip = Cast<ULostArcItemEquipBase>(GetAbility(OwnerIndex));
			if(OwnerEquip->GetAcType() != IndexDecoding(DistIndex, false)) return;
			SetAbility(GetAbility(OwnerIndex,true), DistIndex);
		}
		else // 교체
		{
			auto OwnerEquip = Cast<ULostArcItemEquipBase>(GetAbility(OwnerIndex)), DistEquip = Cast<ULostArcItemEquipBase>(GetAbility(DistIndex));
			if(OwnerEquip->GetAcType() != DistEquip->GetAcType()) return;
			
			Swap(EquipSlot.Find(IndexDecoding(OwnerIndex))->EquipArray[OwnerIndex], EquipSlot.Find(IndexDecoding(DistIndex))->EquipArray[DistIndex]);

			EquipSlotUpdate.Broadcast(IndexEncoding(OwnerEquip->GetAcType(), OwnerIndex));
			EquipSlotUpdate.Broadcast(IndexEncoding(DistEquip->GetAcType(), DistIndex));
		}
	}
	else // 외부 슬롯 스왑
	{
		ILostArcAbilityInterface* OwnerInterface = Cast<ILostArcAbilityInterface>(OwnerComponent);
		if(OwnerInterface == nullptr) return;
		
		auto OwnerEquip = Cast<ULostArcItemEquipBase>(OwnerInterface->GetAbility(OwnerIndex));
		if(OwnerEquip == nullptr) return;
		
		if(GetAbility(DistIndex) == nullptr) // 이동
		{
			if(OwnerEquip->GetAcType() == IndexDecoding(DistIndex))
			{
				SetAbility(OwnerInterface->GetAbility(OwnerIndex, true), IndexEncoding(OwnerEquip->GetAcType(), DistIndex));
			}
		}
		else // 교체
		{
			if(Cast<ULostArcItemEquipBase>(GetAbility(DistIndex))->GetAcType() == OwnerEquip->GetAcType()) // 같은 장비류인 경우
			{
				auto OwnerData = OwnerInterface->GetAbility(OwnerIndex,true);
				if(OwnerData && OwnerInterface->SetAbility(GetAbility(DistIndex), OwnerIndex))
				{
					Cast<ULostArcItemEquipBase>(GetAbility(DistIndex))->Dismount(Cast<ALostArcPlayerCharacter>(GetOwner()));
					SetAbility(OwnerData, DistIndex);
				}
			}
		}
	}
}
bool ULostArcCharacterEquipComponent::SetAbility(ULostArcAbilityBase* OwnerAbility, int32 SlotIndex)
{
	auto OwnerEquip = Cast<ULostArcItemEquipBase>(OwnerAbility);
	if(OwnerEquip == nullptr) return false;
	
	if(SlotIndex == -1)
	{
		for (int32 i = 0; i < *EquipMaxSlot.Find(OwnerEquip->GetAcType()); i++) // EquipSlot has Empty
		{
			if(EquipSlot.Find(OwnerEquip->GetAcType())->EquipArray[i] == nullptr)
			{
				EquipSlot.Find(OwnerEquip->GetAcType())->EquipArray[i] = OwnerEquip;
				EquipSlotUpdate.Broadcast(IndexEncoding(OwnerEquip->GetAcType(), i));
				return true;
			}
		}
		
		// EquipSlot has Fully
		Swap(EquipSlot.Find(OwnerEquip->GetAcType())->EquipArray[0], OwnerEquip);
		if(Interface->SetAbility(OwnerEquip)) // 스왑 성공
		{
			OwnerEquip->Dismount(Cast<ALostArcPlayerCharacter>(GetOwner()));
			EquipSlotUpdate.Broadcast(IndexEncoding(OwnerEquip->GetAcType(), 0));
			return true;
		}
		else // 스왑 실패
		{
			Swap(EquipSlot.Find(OwnerEquip->GetAcType())->EquipArray[0], OwnerEquip);
			OwnerEquip->Dismount(Cast<ALostArcPlayerCharacter>(GetOwner()));
			return false;
		}
	}
	else // 인덱스에 삽입
	{
		if(IndexDecoding(SlotIndex, false) == OwnerEquip->GetAcType())
		{
			OwnerEquip->Equipment(Cast<ALostArcPlayerCharacter>(GetOwner()));
			EquipSlot.Find(IndexDecoding(SlotIndex))->EquipArray[SlotIndex] = OwnerEquip;
			EquipSlotUpdate.Broadcast(IndexEncoding(OwnerEquip->GetAcType(), SlotIndex));
			return true;
		}
		return false;
	}
}
ULostArcAbilityBase* ULostArcCharacterEquipComponent::GetAbility(int32 SlotIndex, bool bTrans)
{
	if(bTrans)
	{
		auto TransUnit = Cast<ULostArcItemEquipBase>(GetAbility(SlotIndex));
		
		if(TransUnit != nullptr)
		{
			TransUnit->Dismount(Cast<ALostArcPlayerCharacter>(GetOwner()));
			EquipSlot.Find(IndexDecoding(SlotIndex))->EquipArray[SlotIndex] = nullptr;
			EquipSlotUpdate.Broadcast(IndexEncoding(TransUnit->GetAcType(), SlotIndex));
			return TransUnit;
		}
	
		return nullptr;
	}
	else
	{
		return EquipSlot.Find(IndexDecoding(SlotIndex))->EquipArray[SlotIndex];
	}
}

bool ULostArcCharacterEquipComponent::EquipSlotEmptyCheck(ULostArcItemEquipBase* CheckItem)
{
	int32 iCurrentSlotCount = 0;
	auto EItemType = CheckItem->GetAcType();
	auto MaxSlotCount = EquipMaxSlot.Find(CheckItem->GetAcType());
	
	for(int i = 0; i < *MaxSlotCount; i++)
	{
		EquipSlot.Find(EItemType)->EquipArray[i] != nullptr ? iCurrentSlotCount++ : iCurrentSlotCount;
	}
	
	return iCurrentSlotCount == *MaxSlotCount ? false : true; 
}

EAccessoryType ULostArcCharacterEquipComponent::IndexDecoding(int32& SlotIndex, bool bTrans)
{
	switch (SlotIndex)
	{
	case 0:
		return EAccessoryType::Necklace;
	case 1:
	case 2:
		if(bTrans)
			SlotIndex-=1;
		return EAccessoryType::Earring;
	case 3:
	case 4:
		if(bTrans)
			SlotIndex-=3;
		return EAccessoryType::Ring;
	}
	
	return EAccessoryType::Necklace;
}
int32 ULostArcCharacterEquipComponent::IndexEncoding(EAccessoryType AcType, int32 Index) 
{
	switch (AcType)
	{
	case EAccessoryType::Necklace:
		return Index;
	case EAccessoryType::Earring:
		return Index+=1;
	case EAccessoryType::Ring:
		return Index+=3;
	}
	
	return -1;
}
