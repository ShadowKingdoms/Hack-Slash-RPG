// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/AI/AISkill_BasicAttack.h"
#include "AnimInstances/BossMonsterAnimInstance.h"
#include "Character/MonsterCharacterBase.h"

UAISkill_BasicAttack::UAISkill_BasicAttack(const FObjectInitializer& ObjectInitializer)
{
	SkillRadius.Key = 400.0f;
	SkillRadius.Value = 120.f;
	DamageRatio = 1.f;
	SkillType = EAbilityType::BasicAttack;
}

bool UAISkill_BasicAttack::Use(AMonsterCharacterBase* Monster)
{
	if(Super::Use(Monster))
	{
		auto BossMonsterAnim = Cast<UBossMonsterAnimInstance>(Monster->GetMonsterAnim());
		if(IsValid(BossMonsterAnim))
		{
			BossMonsterAnim->PlayAttackMontage();
			return true;
		}
	}
	return false;
}


