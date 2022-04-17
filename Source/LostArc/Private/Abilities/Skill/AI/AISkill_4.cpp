// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/AI/AISkill_4.h"
#include "AnimInstances/BossMonsterAnimInstance.h"
#include "Character/BossMonsterCharacter.h"
#include "Character/MonsterCharacterBase.h"
#include "Components/CapsuleComponent.h"

UAISkill_4::UAISkill_4(const FObjectInitializer& ObjectInitializer)
{
	DamageRatio = 10.0f;
	SkillRadius.Key = 800.f;
	SkillRadius.Value = 360.f;
	SkillType = EAbilityType::MeleeSkill_4;
}

bool UAISkill_4::Use(AMonsterCharacterBase* Monster)
{
	if(Super::Use(Monster))
	{
		auto BossMonsterAnim = Cast<UBossMonsterAnimInstance>(Monster->GetMonsterAnim());
		auto BossMonster = Cast<ABossMonsterCharacter>(Monster);
		
		Monster->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BossMonster->bBossIsDisable = true;
		BossMonsterAnim->Montage_Play(BossMonsterAnim->BossSkill_4_Montage, 1.f);
		return true;
	}
	else
		return false;
}