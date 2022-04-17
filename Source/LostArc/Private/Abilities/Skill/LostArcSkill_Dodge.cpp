// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/LostArcSkill_Dodge.h"
#include "Components/CapsuleComponent.h"

ULostArcSkill_Dodge::ULostArcSkill_Dodge(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CoolDown = 10.f;
	ManaCost = 0.f;
	SkillRatio = 0.f;
	SkillRadius.Key = 0.f;
	SkillRadius.Value = 0.f;
	Skill_Type = EAbilityType::Evade;
	Name = "Skill_Dodge";
}

bool ULostArcSkill_Dodge::Use(ALostArcPlayerCharacter* Character)
{
	if (AbilityStateCheck(Character))
	{
		PreCast(Character);
		Character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("ArcCharacterEvade"));
		Character->AnimInstance->Montage_Play(Character->AnimInstance->Evade_Montage, 1.f);
		return true;
	}
	else
		return false;
}

bool ULostArcSkill_Dodge::AbilityStateCheck(ALostArcPlayerCharacter* Character)
{
	if (Character->GetWorldTimerManager().IsTimerActive(AbilityCDProperty.Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("CoolDown Left is : %f"), Character->GetWorldTimerManager().GetTimerRemaining(AbilityCDProperty.Key));
		return false;
	}
	if (Character->StatComponent->GetCurrentAttributeValue(EAttributeType::MP) < ManaCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Mana is : %f"), Character->StatComponent->GetCurrentAttributeValue(EAttributeType::MP));
		return false;
	}
	return true;
}
