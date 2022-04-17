// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase_RangedBase.h"
#include "LostArcSkill_7.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcSkill_7 : public ULostArcSkillBase_RangedBase
{
	GENERATED_BODY()

public:
	ULostArcSkill_7(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
};
