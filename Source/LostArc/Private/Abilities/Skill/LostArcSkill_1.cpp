// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/LostArcSkill_1.h"


ULostArcSkill_1::ULostArcSkill_1(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CoolDown = 10.f;
	ManaCost = 10.f;
	SkillRatio = 1.5f;
	SkillRadius.Key = 500.f;
	SkillRadius.Value = 180.f;
	Skill_Type = EAbilityType::MeleeSkill_1;
	Name = "Skill_1";
}

bool ULostArcSkill_1::Use(ALostArcPlayerCharacter* Character)
{
	if (Super::Use(Character))
	{
		Character->AnimInstance->Montage_Play(Character->AnimInstance->PlayerMeleeSkill_1_Montage, 1.f);
		return true;
	}
	else
		return false;
}
