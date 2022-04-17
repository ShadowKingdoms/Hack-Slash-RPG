// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "LostArcSkill_1.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcSkill_1 : public ULostArcSkillBase
{
	GENERATED_BODY()

public:
	ULostArcSkill_1(const FObjectInitializer& ObjectInitializer);
	virtual bool Use( ALostArcPlayerCharacter* Character) override;
};
