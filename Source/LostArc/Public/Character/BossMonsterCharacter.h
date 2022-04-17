// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MonsterCharacterBase.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "BossMonsterCharacter.generated.h"

UENUM(BlueprintType)
enum EBossState
{
	Default,
	Casting,
	Groggy 
};

UCLASS(Blueprintable)
class LOSTARC_API ABossMonsterCharacter : public AMonsterCharacterBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool bBossIsDisable;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAbsent;

	UPROPERTY(BlueprintReadWrite) // 블프에서 사용하려면 TEnumAsByte로 선언
	TEnumAsByte<EBossState> BossState;
	
	ABossMonsterCharacter();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	class UAIAbilityComponent* GetAbilityComponent() { return AbilityComponent; }
	virtual float GetBasicAttackRange() override;
	virtual void MonsterAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void BossMonsterAttackHitCheck();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MonsterJumpEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MonsterMeteorEffect();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MonsterBreathEffect();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AActor>> IndicatorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	class UAIAbilityComponent* AbilityComponent;
};
