// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/LostArcSkill_5.h"
#include "Component/LostArcCharacterAbilityComponent.h"

ULostArcSkill_5::ULostArcSkill_5(const FObjectInitializer& ObjectInitializer)
{
	CoolDown = 5.f;
	ManaCost = 30.f;
	SkillRatio = 0.3f;
	SkillRadius.Key = 300.f;
	SkillRadius.Value = 360.f;
	Skill_Type = EAbilityType::RangedSpell_1;
	Name = "Skill_5";
}

bool ULostArcSkill_5::Use(ALostArcPlayerCharacter* Character)
{
	if(Super::Use(Character))
	{
		FHitResult TraceHitResult;
		auto PController = Character->GetNetOwningPlayer()->GetPlayerController(GetWorld());
		PController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		HitLocation = TraceHitResult.Location;
		
		SpawnLocation = FVector(TraceHitResult.Location.X, TraceHitResult.Location.Y, TraceHitResult.Location.Z);

		Character->AnimInstance->Montage_Play(Character->AnimInstance->PlayerRangedSkill_1_Montage, 1.f);
		return true;
	}
	
	return false;
}
