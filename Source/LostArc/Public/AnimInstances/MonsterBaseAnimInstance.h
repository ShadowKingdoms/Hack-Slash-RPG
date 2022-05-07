// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "MonsterBaseAnimInstance.generated.h"

UCLASS()
class LOSTARC_API UMonsterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	bool bMonsterIsDead;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* MonsterDeathMontage;
	
	UMonsterBaseAnimInstance();
	virtual void PlayAttackMontage();
	int32 GetBasicAttackMontageSize();
	void SetDeadAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	TArray<UAnimMontage*> MonsterBasicAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	TArray<UAnimMontage*> MonsterDeathMontages;
};
