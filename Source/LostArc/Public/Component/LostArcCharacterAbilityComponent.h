// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/ActorComponent.h"
#include "LostArcCharacterAbilityComponent.generated.h"

UENUM(BlueprintType)
enum EAbilityType
{
	BasicAttack  UMETA(DisplayName = "Basic"),
	MeleeSkill_1 UMETA(DisplayName = "Melee_1"),
	MeleeSkill_2 UMETA(DisplayName = "Melee_2"),
	MeleeSkill_3 UMETA(DisplayName = "Melee_3"),
	MeleeSkill_4 UMETA(DisplayName = "Melee_4"),
	RangedSpell_1 UMETA(DisplayName = "Ranged_1"),
	RangedSpell_2 UMETA(DisplayName = "Ranged_2"),
	RangedSpell_3 UMETA(DisplayName = "Ranged_3"),
	RangedSpell_4 UMETA(DisplayName = "Ranged_4"),
	Evade		  UMETA(DisplayName = "Evade")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOSTARC_API ULostArcCharacterAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULostArcCharacterAbilityComponent();
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AbilityCast(EAbilityType Type);
	void AbilityHitDetection(EAbilityType Type);
	void RangedSkillEffect(EAbilityType Type);
	void SpawnIndicator(TSubclassOf<AActor> Spawner); // 나중에 블루프린트 등록으로 바꾸기
	void ResetRangedAbilitiesState(EAbilityType CurrentType);
	void SetHiddenIndicator();
	void AbilityCancel();
	void PlayInteraction();
	void CharacterRotateToCursor();
	EAbilityType GetLastType() { return LastRangedType; }
	
	UFUNCTION(BlueprintCallable)
	class ULostArcSkillBase* GetAbilities(EAbilityType Type);

	UFUNCTION()
	void AbilityMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<ULostArcSkillBase>> AbilityClass;
	
	UPROPERTY(meta = (AllowPrivateAccess = true))
	TArray<ULostArcSkillBase*> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AActor>> IndicatorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AActor>> EffectClass;

	TEnumAsByte<EAbilityType> LastRangedType;

	UPROPERTY()
	AActor* Indicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = true))
	AActor* ItemBoxPtr;
};

