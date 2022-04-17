// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Items/LostArcItemBase.h"
#include "Component/LostArcInventoryComponent.h"

bool ULostArcItemBase::IsConsumable() const
{
	if (MaxCount <= 0) // MaxCount가 0 이하면 소모품(무제한)
	{
		return true;
	}
	return false;
}

bool ULostArcItemBase::Use(ALostArcPlayerCharacter* Character)
{
	return AbilityStateCheck(Character);
}

bool ULostArcItemBase::AbilityStateCheck(ALostArcPlayerCharacter* Character)
{
	if(AbilityCDProperty.Key.IsValid())
	{
		if (Character->GetWorldTimerManager().IsTimerActive(AbilityCDProperty.Key))
		{
			UE_LOG(LogTemp, Warning, TEXT("CoolDown Left is : %f"), Character->GetWorldTimerManager().GetTimerRemaining(AbilityCDProperty.Key));
			return false;
		}
	}

	return true;
}

bool ULostArcItemBase::SetItemQuantity(int32 Count)
{
	ItemQuantity += Count;
	QuantityUpdate.Broadcast();

	return bool(FMath::Clamp(ItemQuantity, 0, 1));
}