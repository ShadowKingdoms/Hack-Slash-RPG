// Fill out your copyright notice in the Description page of Project Settings.

#include "Controller/CorpseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"

const FName ACorpseAIController::HomePosKey(TEXT("HomePos"));
const FName ACorpseAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACorpseAIController::TargetKey(TEXT("Target")); // 블랙보드와 연동할 키를 컨트롤러에서 정의해 줌

ACorpseAIController::ACorpseAIController()
{
	
}

void ACorpseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (UseBlackboard(BBAsset, Blackboard)) // create a Blackboard component
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation()); // HomePosKey is current position of Pawn
		if (!RunBehaviorTree(BTAsset)) // create a BehaviorTree component
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run Behavior tree!"));
		}
	}
}

void ACorpseAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACorpseAIController::OnRepeatTimer()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation NavLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NavLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NavLocation.Location);
	}
}
