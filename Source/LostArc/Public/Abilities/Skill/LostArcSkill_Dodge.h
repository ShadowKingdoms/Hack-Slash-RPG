// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "LostArcSkill_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcSkill_Dodge : public ULostArcSkillBase
{
	GENERATED_BODY()
	
public:
	ULostArcSkill_Dodge(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
	virtual bool AbilityStateCheck(ALostArcPlayerCharacter* Character) override;
};
