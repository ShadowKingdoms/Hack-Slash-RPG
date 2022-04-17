// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/AI/AISkillBase.h"
#include "AISkill_3.generated.h"

UCLASS()
class LOSTARC_API UAISkill_3 : public UAISkillBase
{
	GENERATED_BODY()

public:
	UAISkill_3(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(AMonsterCharacterBase* Monster) override;
};
