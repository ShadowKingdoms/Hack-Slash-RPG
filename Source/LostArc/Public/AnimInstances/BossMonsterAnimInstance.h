// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/MonsterBaseAnimInstance.h"
#include "BossMonsterAnimInstance.generated.h"

UCLASS(Blueprintable)
class LOSTARC_API UBossMonsterAnimInstance : public UMonsterBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* BossSkill_1_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* BossSkill_2_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* BossSkill_3_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* BossSkill_4_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* Boss_Groggy_Montage;
	
	UBossMonsterAnimInstance();
	virtual void PlayAttackMontage() override;
	
	UFUNCTION()
	virtual void AnimNotify_Skill_2_EffectCheck();

	UFUNCTION()
	virtual void AnimNotify_Skill_3_EffectCheck();

	UFUNCTION()
	virtual void AnimNotify_Skill_4_EffectCheck(); 
};
