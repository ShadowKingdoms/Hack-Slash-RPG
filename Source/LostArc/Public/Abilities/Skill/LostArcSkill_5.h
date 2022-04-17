// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase_RangedBase.h"
#include "LostArcSkill_5.generated.h"

UCLASS()
class LOSTARC_API ULostArcSkill_5 : public ULostArcSkillBase_RangedBase
{
	GENERATED_BODY()

public:
	ULostArcSkill_5(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
};