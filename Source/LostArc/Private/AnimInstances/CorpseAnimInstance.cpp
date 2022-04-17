// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/CorpseAnimInstance.h"
#include "Character/CorpseCharacter.h"

void UCorpseAnimInstance::PlayCorpseMontage()
{
	Montage_Play(CorpseFlinchMontage, 1.f);
}

void UCorpseAnimInstance::PlayDeathMontage()
{
	Montage_Play(CorpseDeathMontage, 1.1f);
	TryGetPawnOwner()->SetActorEnableCollision(false);
	TryGetPawnOwner()->SetLifeSpan(2.35f);
	TryGetPawnOwner()->GetController()->AController::Destroy();
}

void UCorpseAnimInstance::PlayCorpseFlyawayMontage(float MonsterHP)
{
	if (MonsterHP <= 0.f) // is Dead
	{
		Montage_Play(CorpseFlyawayMontage, 1.1f);
		TryGetPawnOwner()->SetActorEnableCollision(false);
		TryGetPawnOwner()->SetLifeSpan(2.3f);
		TryGetPawnOwner()->GetController()->AController::Destroy();
	}
	else
	{
		Montage_Play(CorpseFlyawayMontage, 1.f);
	}
}

void UCorpseAnimInstance::PlayCorpseDamageHandlingMontage(float MonsterHP)
{
	if (MonsterHP <= 0.f) // is Dead
	{
		SetDeadAnim();
		TryGetPawnOwner()->SetActorEnableCollision(false);
		TryGetPawnOwner()->GetController()->AController::Destroy();
		TryGetPawnOwner()->SetLifeSpan(3.f);
	}
	else
	{
		Montage_Play(CorpseFlinchMontage, 1.f);
	}
}

void UCorpseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCorpseAnimInstance::PlayAttackMontage()
{
	Montage_Play(CorpseAttackMontage, 1.f);
}

void UCorpseAnimInstance::AnimNotify_CorpseAttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}