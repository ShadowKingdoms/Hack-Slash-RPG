// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/LostArcSkillBase_RangedBase.h"
#include "Component/LostArcCharacterAbilityComponent.h"

bool ULostArcSkillBase_RangedBase::Use(ALostArcPlayerCharacter* Character)
{
	auto AComponent = Character->AbilityComponent;
	switch (Skill_State)
	{
	case EAbilityState::Stand:
		if(AbilityStateCheck(Character))
		{
			ActivityIndicator(Character);
			return false;
		}
		break;
	case EAbilityState::Focusing:
		if(AbilityStateCheck(Character))
		{
			AComponent->SetHiddenIndicator();
			SetState(EAbilityState::Stand);
			return Super::Use(Character); // 스킬 시전
		}
		break;
	default:
		break;
	}
	
	return false;
}

void ULostArcSkillBase_RangedBase::ActivityIndicator(ALostArcPlayerCharacter* Character)
{
	auto AComponent = Character->AbilityComponent;
	
	Skill_State = EAbilityState::Focusing;
	AComponent->ResetRangedAbilitiesState(Skill_Type); // 다른 스킬 상태 초기화
	AComponent->SpawnIndicator(Skill_Indicator); // 스킬 커서 변환
}

void ULostArcSkillBase_RangedBase::SpawnEffect(ALostArcPlayerCharacter* Character)
{
	OnRangedEffectCheck.Broadcast(Skill_Type);
}

void ULostArcSkillBase_RangedBase::ActorHitDetection(FVector Location, ALostArcPlayerCharacter* Character)
{
	HitLocation = Location;
	HitDetection(Character);
}

void ULostArcSkillBase_RangedBase::HitDetection(ALostArcPlayerCharacter* Character)
{
	TPair<FCollisionQueryParams, TArray<FHitResult>> HitResultProperty(FCollisionQueryParams(NAME_None, false, Character), TArray<FHitResult>());
	GetWorld()->SweepMultiByChannel(HitResultProperty.Value, HitLocation, HitLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(SkillRadius.Key), HitResultProperty.Key);
	
	for (int32 i = 0; i < HitResultProperty.Value.Num(); i++)
	{
		FHitResult hit = HitResultProperty.Value[i];
		FDamageEvent DamageEvent;
		if (hit.Actor.IsValid())
		{
			UE_LOG(LogTemp,Warning,TEXT("Taking Damage : %f"), Character->StatComponent->GetCurrentAttributeValue(EAttributeType::ATK) * SkillRatio);
			hit.Actor->TakeDamage(Character->StatComponent->GetCurrentAttributeValue(EAttributeType::ATK) * SkillRatio, DamageEvent, Character->GetController(), Character);
		}
	}
}