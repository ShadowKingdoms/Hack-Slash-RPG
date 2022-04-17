// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "UObject/NoExportTypes.h"
#include "Character/BossMonsterCharacter.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "AISkillBase.generated.h"

UCLASS(Blueprintable)
class LOSTARC_API UAISkillBase : public UObject
{
	GENERATED_BODY()

public:
	static bool bMonsterAnimationRunning;
	
	virtual bool Use(class AMonsterCharacterBase* Monster);
	virtual bool AbilityStateCheck(class AMonsterCharacterBase* Monster);
	TPair<float, float> GetSkillRadius() {return SkillRadius;}
	void SetIndicator(TSubclassOf<AActor> Indicator);

protected:
	TEnumAsByte<EAbilityType> SkillType;
	TPair<float, float> SkillRadius; // Distance, Angle
	float DamageRatio;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> Skill_Indicator;
};
