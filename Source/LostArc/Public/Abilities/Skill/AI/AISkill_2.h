// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/AI/AISkillBase.h"
#include "AISkill_2.generated.h"

UCLASS()
class LOSTARC_API UAISkill_2 : public UAISkillBase
{
	GENERATED_BODY()

public:
	UAISkill_2(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(AMonsterCharacterBase* Monster) override;
};
