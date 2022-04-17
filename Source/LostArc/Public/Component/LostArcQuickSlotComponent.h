#pragma once

#include "CoreMinimal.h"
#include "LostArcAbilityInterface.h"
#include "Components/ActorComponent.h"
#include "LostArcQuickSlotComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTARC_API ULostArcQuickSlotComponent : public UActorComponent, public ILostArcAbilityInterface
{
	GENERATED_BODY()

public:	
	ULostArcQuickSlotComponent();
	virtual void UseAbility(int32 SlotIndex) override;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TArray<class ULostArcAbilityBase*> QuickSlot;
	
	const int32 MaxSlotCount = 16;

	virtual ULostArcAbilityBase* GetAbility(int32 SlotIndex, bool bTrans = false) override;
};
