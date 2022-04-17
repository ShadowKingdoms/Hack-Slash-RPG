// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Items/Equip/LostArcItemEquip_Necklace.h"


ULostArcItemEquip_Necklace::ULostArcItemEquip_Necklace(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Name = FString("Equip_Necklace");
	AcType = EAccessoryType::Necklace;
	Value = 50.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T2D_ICON(TEXT("Texture2D'/Game/Icons/Item/Equip/acc_209.acc_209'"));
	if (T2D_ICON.Object != NULL)
	{
		Ability_Icon = T2D_ICON.Object;
	}
}

bool ULostArcItemEquip_Necklace::Use(ALostArcPlayerCharacter* Character)
{
	if (Super::Use(Character))
	{
		auto StatComponent = Character->StatComponent;
		StatComponent->AddBonusAttribute(EAttributeType::HP, Value);
		return true;
	}
	
	return false;
}

void ULostArcItemEquip_Necklace::Equipment(ALostArcPlayerCharacter* Character)
{
	Super::Equipment(Character);
	auto StatComponent = Character->StatComponent;
	StatComponent->AddBonusAttribute(EAttributeType::HP, Value);
}

void ULostArcItemEquip_Necklace::Dismount(ALostArcPlayerCharacter* Character)
{
	Super::Dismount(Character);
	auto StatComponent = Character->StatComponent;
	StatComponent->AddBonusAttribute(EAttributeType::HP, -Value);
}
