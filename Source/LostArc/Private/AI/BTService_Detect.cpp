// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Character/BossMonsterCharacter.h"
#include "Character/LostArcPlayerCharacter.h"
#include "Controller/MonsterBaseAIController.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f; // 서비스 호출 주기
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterCharacterBase* ControllingPawn = Cast<AMonsterCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return;
	
	ECollisionChannel PawnChannel = ECollisionChannel::ECC_GameTraceChannel7;

	const auto bIsBoss = Cast<ABossMonsterCharacter>(ControllingPawn);
	if(bIsBoss != nullptr)
	{
		if(bIsBoss->bBossIsDisable)
		{
			PawnChannel = ECollisionChannel::ECC_GameTraceChannel5;
		}
	}

	const UWorld* World = ControllingPawn->GetWorld();
	const FVector Center = ControllingPawn->GetActorLocation();
	const FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);
	TArray<FOverlapResult> OverlapResults;

	const bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, PawnChannel, FCollisionShape::MakeSphere(ControllingPawn->GetMonsterDetectRadius()), CollisionQueryParams);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			const auto Player = Cast<ALostArcPlayerCharacter>(OverlapResult.GetActor());
			if (Player && Player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterBaseAIController::TargetKey, Player); // TargetKey에 Player 정보를 저장한다. (Value로)
				//DrawDebugSphere(World, Center, DetectRadius, 32, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, Player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), Player->GetActorLocation(), FColor::Blue, false, 0.2f);
				Cast<AMonsterCharacterBase>(ControllingPawn)->ToggleHPBarWidget(true); // HP Bar 활성화
				return;
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterBaseAIController::TargetKey, nullptr);
				// 주변 스캔 결과 플레이어가 없을 경우 TargetKey 값을 null로 초기화해 플레이어를 쫓아가지 않게 한다.
				// (Target Key가 Set되면 Target On 데코레이터가 작동하여 기존의 Target 값을 대상으로 계속 플레이어를 추적하니깐)
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterBaseAIController::TargetKey, nullptr);
	}
	//DrawDebugSphere(World, Center, FDetectRadius, 64, FColor::Red, false, 0.2f);
}
