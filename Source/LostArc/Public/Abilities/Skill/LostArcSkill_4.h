// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "LostArcSkill_4.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARC_API ULostArcSkill_4 : public ULostArcSkillBase
{
	GENERATED_BODY()
	
public:
	ULostArcSkill_4(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
	virtual void HitDetection(ALostArcPlayerCharacter* Character) override;

private:
	UPROPERTY()
	AActor* isEffect;
};
