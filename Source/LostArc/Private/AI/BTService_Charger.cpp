// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_Charger.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/AIAbilityComponent.h"
#include "Controller/BossMonsterAIController.h"
#include "Controller/MonsterBaseAIController.h"

UBTService_Charger::UBTService_Charger()
{
	NodeName = TEXT("Charger");
	Interval = 0.1f; // 서비스 호출 주기
	isContact = false;
}

void UBTService_Charger::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	const auto MonsterCharacter = Cast<ABossMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(MonsterCharacter == nullptr) return;

	if(isContact == false)
	{
		const auto AIComponent = Cast<UAIAbilityComponent>(MonsterCharacter->GetAbilityComponent());
		AIComponent->ResetCDTimer(MonsterCharacter);
		isContact = true;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossMonsterAIController::IsChargeKey,  MonsterCharacter->GetAbilityComponent()->GetCDProperty());
}
