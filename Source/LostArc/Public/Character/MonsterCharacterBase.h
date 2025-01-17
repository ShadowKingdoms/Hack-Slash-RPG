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
	void MonsterHPChanged();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DropPickupItem();

	UFUNCTION(BlueprintImplementableEvent)
	void MonsterHPWidgetHiddenEvent();
	
	virtual void ToggleHPBarWidget(bool bVisibility);
	float GetMonsterDetectRadius() const { return MonsterDetectRadius; }
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MonsterExtinctionEffects();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterHP;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterTempHP;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterMaxHP;
	
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

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MonsterDetectRadius;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsShowHPBar = false;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	class UMonsterBaseAnimInstance* MonsterAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HPBarWidget;
};
