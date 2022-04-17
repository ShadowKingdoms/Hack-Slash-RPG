// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Items/LostArcItemBase.h"
#include "LostArcItemPotionBase.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcItemPotionBase : public ULostArcItemBase
{
	GENERATED_BODY()
	
public:
	ULostArcItemPotionBase(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;

protected:
	virtual bool Consumed(ALostArcPlayerCharacter* Character);
};
