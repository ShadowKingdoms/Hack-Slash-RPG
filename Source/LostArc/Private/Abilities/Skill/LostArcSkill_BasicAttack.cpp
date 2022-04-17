// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/LostArcSkill_BasicAttack.h"

ULostArcSkill_BasicAttack::ULostArcSkill_BasicAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CoolDown = 0.f;
	ManaCost = 0.f;
	SkillRatio = 0.5f;
	SkillRadius.Key = 300.0f;
	SkillRadius.Value = 120.f;
	bBasicAttacking = false;
	CurrentCombo = 0;
	MaxCombo = 3;
	Skill_Type = EAbilityType::BasicAttack;
	Name = "Skill_BasicAttack";
}

bool ULostArcSkill_BasicAttack::Use(ALostArcPlayerCharacter* Character)
{
	if (bBasicAttacking) // stack up the combo and waiting for next section
	{
		check(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (bCanNextCombo)
		{
			bIsComboInputOn = true;
		}
	}
	else // first attack
	{
		if (AbilityStateCheck(Character))
		{
			PreCast(Character);
			Character->AnimInstance->Montage_Play(Character->AnimInstance->BasicAttack_Montage, 1.0f);
			return true;
		}
	}
	return false;
}

bool ULostArcSkill_BasicAttack::AbilityStateCheck(ALostArcPlayerCharacter* Character)
{
	if (bAnimationRunning)
	{
		return false;
	}
	else
		return true;
}

void ULostArcSkill_BasicAttack::BasicAttackStartComboState()
{
	bCanNextCombo = true;
	bIsComboInputOn = false;
	check(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ULostArcSkill_BasicAttack::BasicAttackEndComboState()
{
	bCanNextCombo = false;
	bIsComboInputOn = false;
	bAnimationRunning = false;
	CurrentCombo = 0;
}

void ULostArcSkill_BasicAttack::BasicAttackNextComboCheck(ALostArcPlayerCharacter* Character)
{
	if (bIsComboInputOn)
	{
		BasicAttackStartComboState();
		Character->AnimInstance->JumpToBasicAttackMontageSection(CurrentCombo);
	}
}

void ULostArcSkill_BasicAttack::PreCast(ALostArcPlayerCharacter* Character)
{
	check(CurrentCombo == 0);
	BasicAttackStartComboState();
	CharacterRotateToCursor(Character);
	bBasicAttacking = true;
	bAnimationRunning = true;
}