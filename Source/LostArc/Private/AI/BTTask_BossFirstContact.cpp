// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_BossFirstContact.h"
#include "AIController.h"
#include "Character/BossMonsterCharacter.h"
#include "Component/AIAbilityComponent.h"

EBTNodeResult::Type UBTTask_BossFirstContact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto MonsterCharacter = Cast<ABossMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (MonsterCharacter == nullptr) return EBTNodeResult::Failed;

	const auto AIComponent = Cast<UAIAbilityComponent>(MonsterCharacter->GetAbilityComponent());
	AIComponent->ResetCDTimer(MonsterCharacter);
	UE_LOG(LogTemp,Warning,TEXT("ExecuteTask!"));

	return EBTNodeResult::Succeeded;
}