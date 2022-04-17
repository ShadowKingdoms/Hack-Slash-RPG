// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "Abilities/Skill/AI/AISkillBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/LostArcPlayerCharacter.h"
#include "Character/MonsterCharacterBase.h"
#include "Controller/MonsterBaseAIController.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Trun");
} 

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto MonsterCharacter = Cast<AMonsterCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (MonsterCharacter == nullptr) return EBTNodeResult::Failed;

	auto Target = Cast<ALostArcPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterBaseAIController::TargetKey));
	if (Target == nullptr) return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - MonsterCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	MonsterCharacter->SetActorRotation(FMath::RInterpTo(MonsterCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.f));

	return EBTNodeResult::Succeeded;
}