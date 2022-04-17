// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/LostArcSkill_7.h"

ULostArcSkill_7::ULostArcSkill_7(const FObjectInitializer& ObjectInitializer)
{
	CoolDown = 5.f;
	ManaCost = 30.f;
	SkillRatio = 1.5f;
	SkillRadius.Key = 500.f;
	SkillRadius.Value = 360.f;
	Skill_Type = EAbilityType::RangedSpell_3;
	Name = "Skill_7";
}

bool ULostArcSkill_7::Use(ALostArcPlayerCharacter* Character)
{
	if(Super::Use(Character))
	{
		Character->AnimInstance->Montage_Play(Character->AnimInstance->PlayerRangedSkill_3_Montage, 1.f);
		return true;
	}
	return false;
}
