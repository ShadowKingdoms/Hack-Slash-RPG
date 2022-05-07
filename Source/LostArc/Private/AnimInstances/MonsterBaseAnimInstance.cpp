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
	// const auto RandRange = FMath::RandRange(1, 4);
	//
	// Montage_Play(MonsterDeathMontage, 1.f);
	// Montage_JumpToSection(FName(*FString::Printf(TEXT("Death_%d"), RandRange)), MonsterDeathMontage);
}

int32 UMonsterBaseAnimInstance::GetBasicAttackMontageSize()
{
	return MonsterBasicAttackMontages.Num();
}
