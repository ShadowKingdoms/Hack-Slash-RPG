// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Charger.generated.h"

UCLASS()
class LOSTARC_API UBTService_Charger : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Charger();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
