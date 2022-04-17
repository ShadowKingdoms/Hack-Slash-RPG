
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossMonsterAttack.generated.h"

UCLASS()
class LOSTARC_API UBTTask_BossMonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossMonsterAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
