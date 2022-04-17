// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Items/Potion/LostArcItemPotionBase.h"
#include "LostArcItemPotion_Mana.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcItemPotion_Mana : public ULostArcItemPotionBase
{
	GENERATED_BODY()
	
public:
	ULostArcItemPotion_Mana(const FObjectInitializer& ObjectInitializer);
	virtual bool Consumed(ALostArcPlayerCharacter* Character) override;

private:
	float Value = 100.f;
};
