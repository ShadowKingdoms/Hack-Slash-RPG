// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Skill/AI/AISkillBase.h"
#include "Components/ActorComponent.h"
#include "AIAbilityComponent.generated.h"

enum EAbilityType;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOSTARC_API UAIAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIAbilityComponent();
	void AIAbilityCast(class AMonsterCharacterBase* Monster, bool bIsSkill = false);
	bool GetCDProperty() { return AIAbilityCDProperty.Value; }
	void ResetCDTimer(class AMonsterCharacterBase* Monster);
	AActor*& GetIndicatorActorRef() { return IndicatorRef; } // 포인터 레퍼런스를 반환
	AActor*& GetMeteorActorRef() { return MeteorActorRef; } // 포인터 레퍼런스를 반환
	float GetBasicAttackRange();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UAISkillBase>> AbilityClass;
	
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TArray<UAISkillBase*> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AActor>> IndicatorClass;

	UPROPERTY(BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	AActor* IndicatorRef;

	UPROPERTY(BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	AActor* MeteorActorRef;
	
	TPair<FTimerHandle, bool> AIAbilityCDProperty;
};
