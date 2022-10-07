// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Items/Equip/LostArcItemEquipBase.h"

ULostArcItemEquipBase::ULostArcItemEquipBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ItemType = EItemType::ITEM_Equip;
	MaxCount = 1;
	ItemQuantity = 1;
	CoolDown = 0.f;
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> T2D_ICON(TEXT("Texture2D'/Game/Icons/Item/Equip/Ddu8ENAV0AEMKkh.Ddu8ENAV0AEMKkh'"));
	if (T2D_ICON.Object != nullptr)
	{
		BgTexture2D = T2D_ICON.Object;
	}
}

bool ULostArcItemEquipBase::Use(ALostArcPlayerCharacter* Character)
{
	Character->EquipComponent->SetAbility(this);
	return true;
}

void ULostArcItemEquipBase::ItemConstruct()
{
	if(AcType != EAccessoryType::Necklace)
	{
		Value = FMath::RandRange(1,10);
		
		const auto EType = FMath::RandRange(2, 4);
		AtbType = static_cast<EAttributeType>(EType);
		
		// const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAttributeType"), true);
		// const auto Str = EnumPtr->GetNameByValue(AtbType); // Enum to FName
	}

	else
	{
		Value = FMath::RandRange(30,100);
	}
}
