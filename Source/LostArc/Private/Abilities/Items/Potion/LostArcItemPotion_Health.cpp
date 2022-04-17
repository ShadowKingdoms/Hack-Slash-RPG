// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Items/Potion/LostArcItemPotion_Health.h"

ULostArcItemPotion_Health::ULostArcItemPotion_Health(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Name = FString("Potion_Health");

	static ConstructorHelpers::FObjectFinder<UTexture2D> T2D_ICON(TEXT("Texture2D'/Game/Icons/Item/battle_item_01_7.battle_item_01_7'"));
	if (T2D_ICON.Object != NULL)
	{
		Ability_Icon = T2D_ICON.Object;
	}
}

bool ULostArcItemPotion_Health::Consumed(ALostArcPlayerCharacter* Character)
{
	auto StatComponent = Character->StatComponent;
	StatComponent->SetCurrentAttributeValue(EAttributeType::HP, FMath::Clamp(StatComponent->GetCurrentAttributeValue(EAttributeType::HP) + Value, 1.f, StatComponent->GetMaxAttributeValue(EAttributeType::HP)));

	return Super::Consumed(Character);
}
