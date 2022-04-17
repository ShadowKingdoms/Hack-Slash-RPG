// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterBaseAIController.generated.h"

UCLASS()
class LOSTARC_API AMonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	static const FName HomePosKey;
	static const FName TargetKey;
	
	AMonsterBaseAIController();
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY()
	class UBehaviorTree* BTAsset; // 비헤비어 트리 포인터 

	UPROPERTY()
	class UBlackboardData* BBAsset; // 블랙보드 포인터
};
