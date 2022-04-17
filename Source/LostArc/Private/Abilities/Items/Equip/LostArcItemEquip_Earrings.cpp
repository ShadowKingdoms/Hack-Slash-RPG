// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Items/Equip/LostArcItemEquip_Earrings.h"
#include "Component/LostArcInventoryComponent.h"

ULostArcItemEquip_Earrings::ULostArcItemEquip_Earrings(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Name = FString("Equip_Earrings");
	AcType = EAccessoryType::Earring;
	Value = 3.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T2D_ICON(TEXT("Texture2D'/Game/Icons/Item/Equip/acc_109.acc_109'"));
	if (T2D_ICON.Object != NULL)
	{
		Ability_Icon = T2D_ICON.Object;
	}
}

bool ULostArcItemEquip_Earrings::Use(ALostArcPlayerCharacter* Character)
{
	if (Super::Use(Character))
	{
		auto StatComponent = Character->StatComponent;
		StatComponent->AddBonusAttribute(EAttributeType::ATK, Value);
		return true;
	}
	
	return false;
}

void ULostArcItemEquip_Earrings::Equipment(ALostArcPlayerCharacter* Character)
{
	Super::Equipment(Character);
	auto StatComponent = Character->StatComponent;
	StatComponent->AddBonusAttribute(EAttributeType::ATK, Value);
}

void ULostArcItemEquip_Earrings::Dismount(ALostArcPlayerCharacter* Character)
{
	Super::Dismount(Character);
	auto StatComponent = Character->StatComponent;
	StatComponent->AddBonusAttribute(EAttributeType::ATK, -Value);
}
