// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/LostArcPlayerCharacter.h"
#include "Abilities/Skill/LostArcSkill_BasicAttack.h"
#include "AnimInstances/LostArcCharacterAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Component/LostArcCharacterAbilityComponent.h"
#include "Component/LostArcCharacterEquipComponent.h"
#include "Component/LostArcCharacterStatComponent.h"
#include "Component/LostArcInventoryComponent.h"
#include "Component/LostArcQuickSlotComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Controller/LostArcPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALostArcPlayerCharacter::ALostArcPlayerCharacter()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(45.f, 100.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ArcCharacter"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GREY(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (SK_GREY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GREY.Object);
	}

	StatComponent = CreateDefaultSubobject<ULostArcCharacterStatComponent>(TEXT("STAT"));
	AbilityComponent = CreateDefaultSubobject<ULostArcCharacterAbilityComponent>(TEXT("ABILITY"));
	InventoryComponent = CreateDefaultSubobject<ULostArcInventoryComponent>(TEXT("INVENTORY"));
	EquipComponent = CreateDefaultSubobject<ULostArcCharacterEquipComponent>(TEXT("EQUIP"));
	QuickSlotComponent = CreateDefaultSubobject<ULostArcQuickSlotComponent>(TEXT("QUICK"));
}

void ALostArcPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<ULostArcCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (AnimInstance != nullptr)
	{
		AnimInstance->OnNextBasicAttackCheck.AddLambda([this]()->void { Cast<ULostArcSkill_BasicAttack>(AbilityComponent->GetAbilities(EAbilityType::BasicAttack))->BasicAttackNextComboCheck(this); });
		AnimInstance->OnMeleeSkillHitCheck.AddLambda([this](EAbilityType Type)->void { AbilityComponent->AbilityHitDetection(Type); });
		AnimInstance->OnRangedSkillEffectCheck.AddUObject(AbilityComponent, &ULostArcCharacterAbilityComponent::RangedSkillEffect);
		AnimInstance->OnMontageEnded.AddDynamic(AbilityComponent, &ULostArcCharacterAbilityComponent::AbilityMontageEnded); // ※ AddDynamic 매크로의 바인딩은 해당 클래스 내의 멤버 함수를 대상으로 해야한다. (자주 끊어져서)
	}
	StatComponent->OnHPIsZero.AddLambda([this]()->void {AnimInstance->SetDeadAnim(); SetActorEnableCollision(false); Cast<ALostArcPlayerController>(GetController())->SetInputMode(FInputModeUIOnly()); });
}

void ALostArcPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAction<FBindAbilityDelegate>("BasicAttack", IE_Pressed, AbilityComponent, &ULostArcCharacterAbilityComponent::AbilityCast, EAbilityType::BasicAttack);
	InputComponent->BindAction<FBindAbilityDelegate>("Evade", IE_Pressed, AbilityComponent, &ULostArcCharacterAbilityComponent::AbilityCast, EAbilityType::Evade);
	InputComponent->BindAction("Interaction", IE_Pressed,AbilityComponent, &ULostArcCharacterAbilityComponent::PlayInteraction);

	for(int i = 0; i < 16; i++)
	{
		InputComponent->BindAction<FBindQuickSlotDelegate>(FName(FString::Printf(TEXT("QuickSlot_%d"), i)), IE_Pressed, QuickSlotComponent, &ULostArcQuickSlotComponent::UseAbility, i);
	}
}

void ALostArcPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALostArcPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ALostArcPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	const float Weight = UKismetMathLibrary::RandomFloatInRange(0.7f,1.2f);
	FinalDamage *= Weight;	
	const auto LastDamage = FMath::FloorToInt(FinalDamage);
	
	StatComponent->SetDamage(LastDamage);
	OnPlayerTakeDamage(LastDamage);
	
	return LastDamage;
}