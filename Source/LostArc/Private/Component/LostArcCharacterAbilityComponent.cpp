// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LostArcCharacterAbilityComponent.h"
#include "Component/LostArcCharacterStatComponent.h"
#include "Abilities/LostArcAbilityBase.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "Abilities/Skill/LostArcSkill_BasicAttack.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Abilities/Skill/LostArcSkillBase_RangedBase.h"

// Sets default values for this component's properties
ULostArcCharacterAbilityComponent::ULostArcCharacterAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	AbilityClass.Init(ULostArcSkillBase::StaticClass(), 10);
	IndicatorClass.Init(NULL, 10);
	EffectClass.Init(NULL, 10);
}

void ULostArcCharacterAbilityComponent::InitializeComponent() // Init 컴포넌트는 컴파일 타임에 생성자가 호출된 후에 바로 호출되는 경향이 있기에 미리 AbilityClass를 초기화해줘야한다.
{
	Super::InitializeComponent();
	
	for (int i = 0; i < 10; i++)
	{
		Abilities.Add(NewObject<ULostArcSkillBase>(this, AbilityClass[i].Get())); // Get()은 UClass 원본 데이터(파생형)를 반환한다.
	}
	
	ULostArcSkillBase::ConstructSkillSubActors(Abilities, IndicatorClass, EffectClass); // 원거리 스킬용 액터 생성
}

// Called when the game starts
void ULostArcCharacterAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ULostArcCharacterAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULostArcCharacterAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) // 동적 할당한 멤버를 여기서 해제해준다.
{
	Super::EndPlay(EndPlayReason);
	ULostArcSkillBase::bAnimationRunning = false;
	Abilities.Empty();
}

void ULostArcCharacterAbilityComponent::AbilityCancel()
{
	auto Ability = Cast<ULostArcSkillBase_RangedBase>(Abilities[LastRangedType]);
	if(IsValid(Ability))
	{
		if(Ability->GetState() == EAbilityState::Focusing && IsValid(Indicator))
		{
			Ability->SetState(EAbilityState::Stand);
			Indicator->Destroy();
		}
	}
}

void ULostArcCharacterAbilityComponent::PlayInteraction()
{
	const auto ACharacter = Cast<ALostArcPlayerCharacter>(GetOwner());
	
	if(!ULostArcSkillBase::bAnimationRunning)
	{
		if(ItemBoxPtr != nullptr)
		{
			ULostArcSkillBase::bAnimationRunning = true;
			ACharacter->PlayAnimMontage(ACharacter->AnimInstance->PlayerInteraction_Montage);
			CharacterRotateToCursor();
		}
	}
}

void ULostArcCharacterAbilityComponent::SpawnIndicator(TSubclassOf<AActor> Spawner)
{
	if(IsValid(Spawner))
	{
		Indicator = GetWorld()->SpawnActor<AActor>(Spawner, GetOwner()->GetActorTransform());
	}
}

void ULostArcCharacterAbilityComponent::AbilityCast(EAbilityType Type)
{
	if(Type == EAbilityType::BasicAttack) // 마우스 클릭으로 원거리 스킬 실행
	{
		auto Ability = Cast<ULostArcSkillBase_RangedBase>(Abilities[LastRangedType]);

		if(IsValid(Ability)) // 캐스팅이 되면
		{
			if(Ability->GetState() == EAbilityState::Focusing) // 포커싱 상태일 시
			{
				Abilities[LastRangedType]->Use(Cast<ALostArcPlayerCharacter>(GetOwner())); // 캐스팅
				return;
			}
		}
	}
	Abilities[Type]->Use(Cast<ALostArcPlayerCharacter>(GetOwner()));
}

void ULostArcCharacterAbilityComponent::AbilityHitDetection(EAbilityType Type)
{
	Abilities[Type]->HitDetection(Cast<ALostArcPlayerCharacter>(GetOwner()));
	
}

void ULostArcCharacterAbilityComponent::RangedSkillEffect(EAbilityType Type)
{
	auto Character = Cast<ALostArcPlayerCharacter>(GetOwner());
	auto RangedSkill = Cast<ULostArcSkillBase_RangedBase>(Abilities[Type]);

	if(IsValid(RangedSkill))
	{
		Cast<ULostArcSkillBase_RangedBase>(Abilities[Type])->SpawnEffect(Character);
	}
}

ULostArcSkillBase* ULostArcCharacterAbilityComponent::GetAbilities(EAbilityType Type)
{
	return Abilities[Type];
}

void ULostArcCharacterAbilityComponent::AbilityMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) // Evade
	{
		if (Montage->IsValidSectionName(TEXT("BasicAttack_1")))
		{
			Cast<ULostArcSkill_BasicAttack>(GetAbilities(EAbilityType::BasicAttack))->SetBasicAttacking(false);
			Cast<ULostArcSkill_BasicAttack>(GetAbilities(EAbilityType::BasicAttack))->BasicAttackEndComboState();
		}
		return;
	}

	if (Montage->IsValidSectionName(TEXT("BasicAttack_1")))
	{
		Cast<ULostArcSkill_BasicAttack>(GetAbilities(EAbilityType::BasicAttack))->SetBasicAttacking(false);
		Cast<ULostArcSkill_BasicAttack>(GetAbilities(EAbilityType::BasicAttack))->BasicAttackEndComboState();
	}

	if (Montage->IsValidSectionName(TEXT("Evade")))
	{
		Cast<ALostArcPlayerCharacter>(GetOwner())->GetCapsuleComponent()->SetCollisionProfileName(TEXT("ArcCharacter"));
		ULostArcSkillBase::bAnimationRunning = false;
	}

	if(Montage->IsValidSectionName(TEXT("Interaction")))
	{
		ULostArcSkillBase::bAnimationRunning = false;
	}

	for (int i = 1; i < 5; i++) // melee skill
	{
		if(Montage->IsValidSectionName(FName(FString::Printf(TEXT("MeleeSkill_%d"), i))))
		{
			ULostArcSkillBase::bAnimationRunning = false;
			break;
		}
		else if(Montage->IsValidSectionName(FName(FString::Printf(TEXT("RangedSkill_%d"), i))))
		{
			ULostArcSkillBase::bAnimationRunning = false;
			break;
		}
	}
}

void ULostArcCharacterAbilityComponent::ResetRangedAbilitiesState(EAbilityType CurrentType)
{
	if(LastRangedType != CurrentType)
	{
		if(IsValid(Indicator))
		{
			Indicator->Destroy();
		}
	}
	
	for(int i = 5; i < 9; i++) // ranged skill
	{
		auto Ability = Cast<ULostArcSkillBase_RangedBase>(Abilities[i]);

		if(IsValid(Ability) && Ability->GetSkillType() != CurrentType)
		{
			Ability->SetState(EAbilityState::Stand);
		}
	}
	
	LastRangedType = CurrentType;
}

void ULostArcCharacterAbilityComponent::SetHiddenIndicator()
{
	Indicator->SetActorHiddenInGame(true);
	Indicator->SetActorTickEnabled(false);
}

void ULostArcCharacterAbilityComponent::CharacterRotateToCursor()
{
	FHitResult Hit;
	
	Cast<APlayerController>(GetOwner()->GetInstigatorController())->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	float ang = FMath::Atan2(Hit.ImpactPoint.Y - GetOwner()->GetActorLocation().Y, Hit.ImpactPoint.X - GetOwner()->GetActorLocation().X) * 180 / PI;
	if (ang < 0) ang += 360;

	GetOwner()->SetActorRelativeRotation(FRotator(0.0f, ang, 0.0f));
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetOwner()->GetInstigatorController(), GetOwner());
}
