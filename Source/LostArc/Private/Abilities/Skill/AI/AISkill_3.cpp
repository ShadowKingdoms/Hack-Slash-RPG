// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/AI/AISkill_3.h"
#include "AnimInstances/BossMonsterAnimInstance.h"
#include "Character/MonsterCharacterBase.h"

UAISkill_3::UAISkill_3(const FObjectInitializer& ObjectInitializer)
{
	DamageRatio = 5.0f;
	SkillRadius.Key = 600.f;
	SkillRadius.Value = 90.f;
	SkillType = EAbilityType::MeleeSkill_3;
}

bool UAISkill_3::Use(AMonsterCharacterBase* Monster)
{
	if(Super::Use(Monster))
	{
		auto BossMonsterAnim = Cast<UBossMonsterAnimInstance>(Monster->GetMonsterAnim());
		BossMonsterAnim->Montage_Play(BossMonsterAnim->BossSkill_3_Montage, 1.f);
		return true;
	}
	else
		return false;
}
