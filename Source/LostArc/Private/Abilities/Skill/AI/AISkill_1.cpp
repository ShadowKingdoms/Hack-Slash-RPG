// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/AI/AISkill_1.h"
#include "AnimInstances/BossMonsterAnimInstance.h"
#include "Character/MonsterCharacterBase.h"

UAISkill_1::UAISkill_1(const FObjectInitializer& ObjectInitializer)
{
	DamageRatio = 3.0f;
	SkillRadius.Key = 400.f;
	SkillRadius.Value = 120.f;
	SkillType = EAbilityType::MeleeSkill_1;
}

bool UAISkill_1::Use(AMonsterCharacterBase* Monster)
{
	if(Super::Use(Monster))
	{
		auto BossMonsterAnim = Cast<UBossMonsterAnimInstance>(Monster->GetMonsterAnim());
		BossMonsterAnim->Montage_Play(BossMonsterAnim->BossSkill_1_Montage, 1.f);
		return true;
	}
	else
		return false;
}
