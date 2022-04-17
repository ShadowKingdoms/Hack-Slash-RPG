// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBasicAttackEndDelegate);

UCLASS(Blueprintable)
class LOSTARC_API AMonsterCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	FOnBasicAttackEndDelegate OnBasicAttackEnd;
	
	AMonsterCharacterBase();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	class UMonsterBaseAnimInstance* GetMonsterAnim() { return MonsterAnim; }
	virtual float GetBasicAttackRange();
	virtual void MonsterAttack();
	
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	virtual void TheMonsterAttackHitCheck();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DropPickupItem();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterHP;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterSpeed;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterStr;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float BasicAttackRange;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float BasicAttackRadius;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterRotateSpeed;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	class UMonsterBaseAnimInstance* MonsterAnim;
	
};
