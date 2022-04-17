// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/LostArcSkill_6.h"

ULostArcSkill_6::ULostArcSkill_6(const FObjectInitializer& ObjectInitializer)
{
	CoolDown = 4.f;
	ManaCost = 40.f;
	SkillRatio = 0.2f;
	SkillRadius.Key = 300.f;
	SkillRadius.Value = 360.f;
	Skill_Type = EAbilityType::RangedSpell_2;
	Name = "Skill_6";
}

bool ULostArcSkill_6::Use(ALostArcPlayerCharacter* Character)
{
	if(Super::Use(Character))
	{
		Character->AnimInstance->Montage_Play(Character->AnimInstance->PlayerRangedSkill_2_Montage, 1.f);
		return true;
	}
	return false;
}