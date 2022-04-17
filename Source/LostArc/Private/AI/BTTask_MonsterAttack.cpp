// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_MonsterAttack.h"
#include "Character/MonsterCharacterBase.h"
#include "Controller/MonsterBaseAIController.h"
#include "AnimInstances/MonsterBaseAnimInstance.h"

UBTTask_MonsterAttack::UBTTask_MonsterAttack()
{
	bNotifyTick = true;
	bIsAttacking = false;
}

EBTNodeResult::Type UBTTask_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto MonsterCharacter = Cast<AMonsterCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (MonsterCharacter == nullptr) return EBTNodeResult::Failed;
	
	auto MonsterAnim = Cast<UMonsterBaseAnimInstance>(MonsterCharacter->GetMesh()->GetAnimInstance());
	if (MonsterAnim->Montage_IsPlaying(MonsterAnim->MonsterDeathMontage)) return EBTNodeResult::Failed;

	MonsterCharacter->MonsterAttack();
	bIsAttacking = true;

	MonsterCharacter->OnBasicAttackEnd.AddLambda([this]()->void { bIsAttacking = false; });
	
	return EBTNodeResult::InProgress; // When the task is first run
}

void UBTTask_MonsterAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsAttacking) // UAISkillBase::bMonsterAnimationRunning로 대신하면 보스가 평타 공격을 실행할 때 켜진 플래그가 끝날 때까지 안꺼져서 태스크랑 처음 실행할 때를 포함하여 (바로 InProgress로 넘어가니) 영원히 Succeeded를 반환하지 못하게된다.
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // 태스크가 Succeeded를 반환하지 못하면 ExecuteTask에서 InProgress를 반환하여 기다리게한다. bIsAttacking이 false라면 Succeeded를 반환한다.
	}
}

