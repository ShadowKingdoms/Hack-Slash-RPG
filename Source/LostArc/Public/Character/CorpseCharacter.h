// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CorpseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class LOSTARC_API ACorpseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACorpseCharacter();
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UCorpseAnimInstance* CorpseAnim;
	FOnAttackEndDelegate OnAttackEnd;

	void Attack();
	void AttackHitCheck();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
	// class ULineOfSightComponent* LOSComponent;

private:
	float MonsterHP;
	float AttackRange;
	float AttackRadius;
};
