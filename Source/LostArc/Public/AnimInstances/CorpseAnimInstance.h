// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "CorpseAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate)

UCLASS()
class LOSTARC_API UCorpseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void PlayAttackMontage();
	void PlayCorpseMontage();
	void PlayDeathMontage();
	void PlayCorpseFlyawayMontage(float MonsterHP);
	void PlayCorpseDamageHandlingMontage(float MonsterHP);
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_CorpseAttackHitCheck();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* CorpseFlinchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* CorpseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* CorpseDeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrviateAccess = true))
	UAnimMontage* CorpseFlyawayMontage;

	FOnAttackHitCheckDelegate OnAttackHitCheck;

	void SetDeadAnim() { bIsDead = true; }
};
