// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstances/MonsterBaseAnimInstance.h"

UMonsterBaseAnimInstance::UMonsterBaseAnimInstance()
{
	MonsterBasicAttackMontages.Init(NULL, 4);
}

void UMonsterBaseAnimInstance::PlayAttackMontage()
{
	auto Index = FMath::RandRange(0, GetBasicAttackMontageSize() - 1);
	Montage_Play(MonsterBasicAttackMontages[Index], 1.f);
}

void UMonsterBaseAnimInstance::SetDeadAnim()
{
	bMonsterIsDead = true;
}

int32 UMonsterBaseAnimInstance::GetBasicAttackMontageSize()
{
	return MonsterBasicAttackMontages.Num();
}