// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/MonsterCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "AnimInstances/MonsterBaseAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Component/AIAbilityComponent.h"
#include "Components/WidgetComponent.h"
#include "Controller/MonsterBaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMonsterCharacterBase::AMonsterCharacterBase()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	AIControllerClass = AMonsterBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());
}

void AMonsterCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterAnim = Cast<UMonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (MonsterAnim != nullptr)
	{
		MonsterAnim->OnMontageEnded.AddDynamic(this, &AMonsterCharacterBase::OnAttackMontageEnded);
	}

	ToggleHPBarWidget(false);
}

void AMonsterCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

float AMonsterCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float FFinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (FFinalDamage > 0.f)
	{
		MonsterHPChanged();	
		
		if(MonsterTempHP <= 0.f)
		{
			MonsterAnim->SetDeadAnim();
			MonsterHPWidgetHiddenEvent();
			SetActorEnableCollision(false);
			DropPickupItem();
			GetController()->Destroyed();
		}	
	}
	
	return FFinalDamage;
}

float AMonsterCharacterBase::GetBasicAttackRange()
{
	return BasicAttackRange;
}

void AMonsterCharacterBase::MonsterAttack()
{
	MonsterAnim->PlayAttackMontage();
}

void AMonsterCharacterBase::TheMonsterAttackHitCheck()
{
	FDamageEvent DamageEvent; FHitResult HitResult; FVector direction;
	float dotValue = FMath::Cos(((PI * 2) / 360) * (BasicAttackRadius / 2));
	FCollisionQueryParams Params(NAME_None, false, this);
	auto bResult = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation(),FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(BasicAttackRange), Params);
	
	if(bResult && HitResult.Actor.IsValid())
	{
		direction = HitResult.Actor.Get()->GetActorLocation() - GetActorLocation();
		if (direction.Size() < BasicAttackRange) 
		{
			if (FVector::DotProduct(direction.GetSafeNormal(), GetActorForwardVector()) > dotValue)
			{
				HitResult.Actor->TakeDamage(MonsterStr, DamageEvent, GetController(), this);
			}
		}
	}
}

void AMonsterCharacterBase::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	for(int i = 1; i <= MonsterAnim->GetBasicAttackMontageSize(); i++)
	{
		if (Montage->IsValidSectionName(FName(FString::Printf(TEXT("Attack_%d"), i))))
		{
			OnBasicAttackEnd.Broadcast();
			return;
		}
	}

	if(Montage->IsValidSectionName(FName("Death")))
	{
		
	}
}

void AMonsterCharacterBase::ToggleHPBarWidget(bool bVisibility)
{
	if(!bIsShowHPBar && bVisibility)
	{
		HPBarWidget->SetVisibility(bVisibility);
		bIsShowHPBar = true; 
	}

	else if(!bVisibility)
	{
		HPBarWidget->SetVisibility(bVisibility);
		bIsShowHPBar = false;
	}
}