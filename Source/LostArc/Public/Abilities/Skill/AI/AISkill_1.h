// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/AI/AISkillBase.h"
#include "AISkill_1.generated.h"

UCLASS()
class LOSTARC_API UAISkill_1 : public UAISkillBase
{
	GENERATED_BODY()

public:
	UAISkill_1(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(AMonsterCharacterBase* Monster) override;
};
