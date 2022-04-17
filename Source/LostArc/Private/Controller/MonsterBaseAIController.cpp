// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MonsterBaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

const FName AMonsterBaseAIController::HomePosKey(TEXT("HomePos"));
const FName AMonsterBaseAIController::TargetKey(TEXT("Target")); // 블랙보드와 연동할 키를 컨트롤러에서 정의해 줌

AMonsterBaseAIController::AMonsterBaseAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/Monster/BB_Monster.BB_Monster'")); // 블루프린트의 블랙보드와 연동
	if (BBObject.Succeeded()) { BBAsset = BBObject.Object; }

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/Monster/BT_Monster.BT_Monster'")); // BP의 BT와 연동
	if (BTObject.Succeeded()) { BTAsset = BTObject.Object; }
}

void AMonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard)) // AIController의 멤버인 "블랙보드 컴포넌트"에 연동한 블랙보드 에셋(BBAsset)을 연결
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation()); // HomePosKey에 폰의 현재 위치 저장
		if (!RunBehaviorTree(BTAsset))
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run Behavior tree!"));
	}
}