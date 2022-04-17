// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "LostArcSkillBase_RangedBase.generated.h"

UENUM(BlueprintType)
enum EAbilityState
{
	Stand    UMETA(DisplayName = "Stand"),
	Focusing UMETA(DisplayName = "Focusing")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRangedEffectCheckDelegate, EAbilityType, Type);

UCLASS(Blueprintable)
class LOSTARC_API ULostArcSkillBase_RangedBase : public ULostArcSkillBase
{
	GENERATED_BODY()

public:
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
	TEnumAsByte<EAbilityState> GetState() { return Skill_State; }
	void SetState(TEnumAsByte<EAbilityState> NewState) { Skill_State = NewState; }
	void ActivityIndicator(class ALostArcPlayerCharacter* Character);
	virtual void SpawnEffect(class ALostArcPlayerCharacter* Character);
	virtual void HitDetection(ALostArcPlayerCharacter* Character) override;
	
	UFUNCTION(BlueprintCallable)
	void ActorHitDetection(FVector Location, ALostArcPlayerCharacter* Character);

protected:
	TEnumAsByte<EAbilityState> Skill_State;
	FVector SpawnLocation;
	FVector HitLocation;
	
	UPROPERTY(BlueprintAssignable, VisibleAnywhere)
	FOnRangedEffectCheckDelegate OnRangedEffectCheck;
};