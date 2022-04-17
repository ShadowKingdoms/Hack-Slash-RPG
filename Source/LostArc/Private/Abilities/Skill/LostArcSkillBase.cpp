// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Skill/LostArcSkillBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Component/LostArcCharacterAbilityComponent.h"

bool ULostArcSkillBase::bAnimationRunning = false;

bool ULostArcSkillBase::Use(ALostArcPlayerCharacter* Character)
{
	if (AbilityStateCheck(Character))
	{
		PreCast(Character);
		return true;
	}
	else
		return false;
}

bool ULostArcSkillBase::AbilityStateCheck(ALostArcPlayerCharacter* Character)
{
	if (Character->GetWorldTimerManager().IsTimerActive(AbilityCDProperty.Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("CoolDown Left is : %f"), Character->GetWorldTimerManager().GetTimerRemaining(AbilityCDProperty.Key));
		return false;
	}
	if (Character->StatComponent->GetCurrentAttributeValue(EAttributeType::MP) < ManaCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Mana is : %f"), Character->StatComponent->GetCurrentAttributeValue(EAttributeType::MP));
		return false;
	}
	if (bAnimationRunning)
	{
		return false;
	}
	return true;
}

void ULostArcSkillBase::CharacterRotateToCursor(ALostArcPlayerCharacter* Character)
{
	FHitResult Hit;
	Cast<APlayerController>(Character->GetController())->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	float ang = FMath::Atan2(Hit.ImpactPoint.Y - Character->GetActorLocation().Y, Hit.ImpactPoint.X - Character->GetActorLocation().X) * 180 / PI;
	if (ang < 0) ang += 360;

	Character->SetActorRelativeRotation(FRotator(0.0f, ang, 0.0f));
	UAIBlueprintHelperLibrary::SimpleMoveToActor(Character->GetController(), Character);
}

void ULostArcSkillBase::HitDetection(ALostArcPlayerCharacter* Character)
{
	TPair<FCollisionQueryParams, TArray<FHitResult>> HitResultProperty(FCollisionQueryParams(NAME_None, false, Character), TArray<FHitResult>());
	float dotValue = FMath::Cos(((PI * 2) / 360) * (SkillRadius.Value / 2));; // 스킬 각도에 대한 코사인 값 (중앙선 길이 / 부채꼴 각도의 절반)
	FDamageEvent DamageEvent;
	FVector direction;
	
	GetWorld()->SweepMultiByChannel(HitResultProperty.Value, Character->GetActorLocation(), Character->GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(SkillRadius.Key), HitResultProperty.Key);

	for (int32 i = 0; i < HitResultProperty.Value.Num(); i++)
	{
		FHitResult hit = HitResultProperty.Value[i];
		if (hit.Actor.IsValid())
		{
			direction = hit.Actor.Get()->GetActorLocation() - Character->GetActorLocation();
			if (direction.Size() < SkillRadius.Key) // 타겟과의 거리가 부채꼴 전체 길이보다 짧으며
			{
				if (FVector::DotProduct(direction.GetSafeNormal(), Character->GetActorForwardVector()) > dotValue) // 부채꼴 밑변(중앙선)의 길이를 타겟의 각도와 나눈 값(코사인)이 스킬 각도에 대한 코사인 값보다 크면 부채꼴 영역 안에 위치함
				{
					UE_LOG(LogTemp,Warning,TEXT("Taking Damage : %f"), Character->StatComponent->GetCurrentAttributeValue(EAttributeType::ATK) * SkillRatio);
					hit.Actor->TakeDamage(Character->StatComponent->GetCurrentAttributeValue(EAttributeType::ATK) * SkillRatio, DamageEvent, Character->GetController(), Character);
				}
			}
		}
	}
}

void ULostArcSkillBase::ConstructSkillSubActors(TArray<ULostArcSkillBase*> BaseAbility, TArray<TSubclassOf<AActor>> Indicator, TArray<TSubclassOf<AActor>> Effect)
{
	for(int i = 0; i < 10; i++)
	{
		if(BaseAbility[i] != nullptr)
		{
			if(Indicator[i] != nullptr)
			{
				BaseAbility[i]->Skill_Indicator = Indicator[i];
			}
			if(Effect[i] != nullptr)
			{
				BaseAbility[i]->Skill_Effect = Effect[i];
			}
		}
	}
}

void ULostArcSkillBase::PreCast(ALostArcPlayerCharacter* Character)
{
	CharacterRotateToCursor(Character);
	Character->GetWorldTimerManager().SetTimer(AbilityCDProperty.Key, FTimerDelegate::CreateLambda([=]() {AbilityCDProperty.Value.Broadcast(false); }), CoolDown, false); // 쿨타임 계산
	Character->StatComponent->SetCurrentAttributeValue(EAttributeType::MP, Character->StatComponent->GetCurrentAttributeValue(EAttributeType::MP) - ManaCost); // 마나 계산
	bAnimationRunning = true;
	AbilityCDProperty.Value.Broadcast(true);
	Character->AbilityComponent->ResetRangedAbilitiesState(Skill_Type);
}
