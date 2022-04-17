// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/AIAbilityComponent.h"
#include "TimerManager.h"
#include "Character/BossMonsterCharacter.h"
#include "Controller/BossMonsterAIController.h"

// Sets default values for this component's properties
UAIAbilityComponent::UAIAbilityComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;
	AbilityClass.Init(UAISkillBase::StaticClass(),5);
	IndicatorClass.Init(NULL, 5);
}

void UAIAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (int i = 0; i < 5; i++)
	{
		Abilities.Add(NewObject<UAISkillBase>(this, AbilityClass[i].Get())); // Get()은 UClass 원본 데이터(파생형)를 반환한다.
		Abilities[i]->SetIndicator(IndicatorClass[i].Get());
	}
}

void UAIAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	auto Monster = Cast<AMonsterCharacterBase>(GetOwner());
	ResetCDTimer(Monster);
}

void UAIAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UAISkillBase::bMonsterAnimationRunning = false;
	Abilities.Empty();
}

void UAIAbilityComponent::ResetCDTimer(AMonsterCharacterBase* Monster)
{
	AIAbilityCDProperty.Value = false;
	auto FTimeScale = FMath::FRandRange(4.f, 7.f);
	UE_LOG(LogTemp,Warning,TEXT("Rest Time : %f"), FTimeScale);
	Monster->GetWorldTimerManager().SetTimer(AIAbilityCDProperty.Key, FTimerDelegate::CreateLambda([&]() { AIAbilityCDProperty.Value = true; }), FTimeScale, true); // 쿨타임 계산
}

float UAIAbilityComponent::GetBasicAttackRange()
{
	return Abilities[EAbilityType::BasicAttack]->GetSkillRadius().Key;
}

void UAIAbilityComponent::AIAbilityCast(AMonsterCharacterBase* Monster, bool bIsSkill)
{
	AIAbilityCDProperty.Value = false;
	
	if(bIsSkill)
	{
		int32 Type = FMath::RandRange(1, 4);
		Abilities[Type]->Use(Monster);
	}
	else
	{
		Abilities[EAbilityType::BasicAttack]->Use(Monster);
	}
}