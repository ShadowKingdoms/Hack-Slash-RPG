// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CorpseCharacter.h"
#include "Controller/CorpseAIController.h"
#include "AnimInstances/CorpseAnimInstance.h"
#include "Component/LineOfSightComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACorpseCharacter::ACorpseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	MonsterHP = 100.0f;
	AttackRadius = 50.0f;
	AttackRange = 50.0f;

	AIControllerClass = ACorpseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// LOSComponent = CreateDefaultSubobject<ULineOfSightComponent>(TEXT("LineOfSightComponent"));
	// LOSComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACorpseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACorpseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACorpseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CorpseAnim = Cast<UCorpseAnimInstance>(GetMesh()->GetAnimInstance());
	if (CorpseAnim == nullptr) return;
	CorpseAnim->OnMontageEnded.AddDynamic(this, &ACorpseCharacter::OnAttackMontageEnded);
	CorpseAnim->OnAttackHitCheck.AddUObject(this, &ACorpseCharacter::AttackHitCheck);
}

// Called to bind functionality to input
void ACorpseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACorpseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FFinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (FFinalDamage > 0.f)
	{
		MonsterHP -= FFinalDamage;
		CorpseAnim->PlayCorpseDamageHandlingMontage(MonsterHP);
	}
	return FFinalDamage;
}

void ACorpseCharacter::Attack()
{
	if (!CorpseAnim->Montage_IsPlaying(CorpseAnim->CorpseAttackMontage)) // 같은 몽타주의 중복 재생을 방지하기 위해 한번만 실행되도록 한다.
	{
		CorpseAnim->PlayAttackMontage();
	}
}

void ACorpseCharacter::AttackHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation() + GetActorForwardVector() * 100.0f,
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (bResult)
	{
		if (HitResult.Actor.IsValid()) // If the hit actor is valid
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(20.0f, DamageEvent, GetController(), this);
		}
	}

	//#if ENABLE_DRAW_DEBUG
	//FVector TraceVec = GetActorForwardVector() * AttackRange;
	//FVector Center = (GetActorLocation() + GetActorForwardVector() * 100.0f) + TraceVec * 0.5f; // 끝 점에서 0.5를 곱해주면 중심점이 된다.
	//float HalfHeight = AttackRange * 0.5f + AttackRadius;
	//FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	//FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//float DebugLifeTime = 1.0f;
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	//#endif
}

void ACorpseCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

	if (Montage->IsValidSectionName(TEXT("Death")))
	{
		AActor::Destroy();
	}
	
	if (Montage->IsValidSectionName(TEXT("Attack"))) 
	{
		OnAttackEnd.Broadcast();
	}
}

