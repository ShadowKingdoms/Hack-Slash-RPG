// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Items/Potion/LostArcItemPotionBase.h"
#include "Component/LostArcInventoryComponent.h"

ULostArcItemPotionBase::ULostArcItemPotionBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ItemType = EItemType::ITEM_Potion;
	MaxCount = 0;
	CoolDown = 1.5f;
	ItemQuantity = 0;

	static ConstructorHelpers::FObjectFinder<UTexture2D> T2D_ICON(TEXT("Texture2D'/Game/Icons/Item/blue_layer.blue_layer'"));
	if (T2D_ICON.Object != NULL)
	{
		BgTexture2D = T2D_ICON.Object;
	}
}

bool ULostArcItemPotionBase::Use(ALostArcPlayerCharacter* Character)
{
	if (Super::Use(Character))
	{
		return Consumed(Character);
	}
	
	return false;
}

bool ULostArcItemPotionBase::Consumed(ALostArcPlayerCharacter* Character)
{
	if (Super::SetItemQuantity(-1)) // Item이 1이상 남음
	{
		Character->GetWorldTimerManager().SetTimer(AbilityCDProperty.Key, FTimerDelegate::CreateLambda([=]() { AbilityCDProperty.Value.Broadcast(false); }), CoolDown, false);
		AbilityCDProperty.Value.Broadcast(true);
		Character->CharacterConsumeEffect();
		return false;
	}
	else
	{
		Character->CharacterConsumeEffect();
		return true; // Item을 모두 소모
	}
}