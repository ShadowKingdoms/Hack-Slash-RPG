// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Skill/AI/AISkillBase.h"
#include "NavigationSystem.h"
#include "Character/BossMonsterCharacter.h"
#include "Character/MonsterCharacterBase.h"
#include "Component/AIAbilityComponent.h"

bool UAISkillBase::bMonsterAnimationRunning = false;

bool UAISkillBase::Use(AMonsterCharacterBase* Monster)
{
	if(AbilityStateCheck(Monster))
	{
		auto Boss = Cast<ABossMonsterCharacter>(Monster);
		if(Skill_Indicator != nullptr)
		{
			auto Component = Boss->GetAbilityComponent();
			auto Transform = Monster->GetMesh()->GetComponentTransform();
			Transform.SetRotation(Monster->GetActorRotation().Quaternion());
			Component->GetIndicatorActorRef() = GetWorld()->SpawnActor<AActor>(Skill_Indicator, Transform);
		}
		
		Boss->BossState = EBossState::Casting;
		return true;
	}
	else
	{
		return false;
	}
}


bool UAISkillBase::AbilityStateCheck(AMonsterCharacterBase* Monster)
{
	if(bMonsterAnimationRunning)
	{
		return false;
	}
	return true;
}

void UAISkillBase::SetIndicator(TSubclassOf<AActor> Indicator)
{
	if(Indicator != nullptr)
	{
		Skill_Indicator = Indicator;
	}
}