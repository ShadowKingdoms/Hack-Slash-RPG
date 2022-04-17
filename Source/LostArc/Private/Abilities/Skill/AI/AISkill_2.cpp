// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/AI/AISkill_2.h"
#include "AnimInstances/BossMonsterAnimInstance.h"
#include "Character/MonsterCharacterBase.h"

UAISkill_2::UAISkill_2(const FObjectInitializer& ObjectInitializer)
{
	DamageRatio = 2.0f;
	SkillRadius.Key = 800.f;
	SkillRadius.Value = 360.f;
	SkillType = EAbilityType::MeleeSkill_2;
}

bool UAISkill_2::Use(AMonsterCharacterBase* Monster)
{
	if(Super::Use(Monster))
	{
		auto BossMonsterAnim = Cast<UBossMonsterAnimInstance>(Monster->GetMonsterAnim());
		BossMonsterAnim->Montage_Play(BossMonsterAnim->BossSkill_2_Montage, 1.f);
		return true;
	}
	else
		return false;
}
