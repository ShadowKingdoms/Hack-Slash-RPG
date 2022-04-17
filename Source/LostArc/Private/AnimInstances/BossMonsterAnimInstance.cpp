// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/BossMonsterAnimInstance.h"
#include "Character/BossMonsterCharacter.h"

UBossMonsterAnimInstance::UBossMonsterAnimInstance()
{
	MonsterBasicAttackMontages.Init(NULL, 3);
}

void UBossMonsterAnimInstance::PlayAttackMontage()
{
	auto Index = FMath::RandRange(0, GetBasicAttackMontageSize() - 1);
	Montage_Play(MonsterBasicAttackMontages[Index], 1.f);
}

void UBossMonsterAnimInstance::AnimNotify_Skill_2_EffectCheck()
{
    auto BossMonster = Cast<ABossMonsterCharacter>(GetOwningActor());
	BossMonster->MonsterMeteorEffect();
}

void UBossMonsterAnimInstance::AnimNotify_Skill_3_EffectCheck()
{
	auto BossMonster = Cast<ABossMonsterCharacter>(GetOwningActor());
	BossMonster->MonsterBreathEffect();
}

void UBossMonsterAnimInstance::AnimNotify_Skill_4_EffectCheck()
{
	auto BossMonster = Cast<ABossMonsterCharacter>(GetOwningActor());
	BossMonster->MonsterJumpEffect();
}