// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/AI/AISkillBase.h"
#include "AISkill_BasicAttack.generated.h"

UCLASS()
class LOSTARC_API UAISkill_BasicAttack : public UAISkillBase
{
	GENERATED_BODY()

public:
	UAISkill_BasicAttack(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(AMonsterCharacterBase* Monster) override;
};
