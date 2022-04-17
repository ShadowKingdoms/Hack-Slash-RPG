// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase_RangedBase.h"
#include "LostArcSkill_8.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcSkill_8 : public ULostArcSkillBase_RangedBase
{
	GENERATED_BODY()
	
public:
	ULostArcSkill_8(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
	virtual void SpawnEffect(ALostArcPlayerCharacter* Character) override;
};
