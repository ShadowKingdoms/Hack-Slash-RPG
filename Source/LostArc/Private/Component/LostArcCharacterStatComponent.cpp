// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LostArcCharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"

ULostArcCharacterStatComponent::ULostArcCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	IncreasedManaRegeneration = 5.0f;
}

void ULostArcCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	const auto ArcGameInstance = Cast<ULostArcGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (ArcGameInstance != nullptr)
	{
		CurrentStatData = ArcGameInstance->GetArcCharacterStatData(1);
		if (CurrentStatData != nullptr)
		{
			SetCurrentAttributeValue(EAttributeType::HP, CurrentStatData->MaxHP);
			SetCurrentAttributeValue(EAttributeType::MP, CurrentStatData->MaxMP);
			SetCurrentAttributeValue(EAttributeType::ATK, CurrentStatData->Attack);
			SetCurrentAttributeValue(EAttributeType::DEF, CurrentStatData->Defense);
			SetCurrentAttributeValue(EAttributeType::CRT, CurrentStatData->Critical);
			
			OnProgressBarChanged.Broadcast(EAttributeType::HP);
			OnProgressBarChanged.Broadcast(EAttributeType::MP);
		}
	}
}

void ULostArcCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	ManaRegenerationTimerDelegate.BindUFunction(this, FName("ManaRegenerationPerSecond"), IncreasedManaRegeneration);
	GetOwner()->GetWorldTimerManager().SetTimer(ManaRegenerationTimerHandle, ManaRegenerationTimerDelegate, 2.0f, true);
}

void ULostArcCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float ULostArcCharacterStatComponent::GetCurrentAttributeValue(EAttributeType Type)
{
	switch (Type)
	{
	case HP:
		return CurrentHP;
		break;
	case MP:
		return CurrentMP;
		break;
	case ATK:
		return CurrentATK;
		break;
	case DEF:
		return CurrentDEF;
		break;
	case CRT:
		return CurrentCritical;
	default:
		break;
	}
	return NULL;
}

float ULostArcCharacterStatComponent::GetMaxAttributeValue(EAttributeType Type)
{
	switch (Type)
	{
	case HP:
		return CurrentStatData->MaxHP + BonusMaxHP;
		break;
	case MP:
		return CurrentStatData->MaxMP + BonusMaxMP;
		break;
	default:
		break;
	}
	return NULL;
}

float ULostArcCharacterStatComponent::GetCurrentAttributeRatio(EAttributeType Type)
{
	switch (Type)
	{
	case HP:
		return GetMaxAttributeValue(EAttributeType::HP) < KINDA_SMALL_NUMBER ? 0.0f : (CurrentHP / GetMaxAttributeValue(EAttributeType::HP));
		break;
	case MP:
		return GetMaxAttributeValue(EAttributeType::MP) < KINDA_SMALL_NUMBER ? 0.0f : (CurrentMP / GetMaxAttributeValue(EAttributeType::MP));
		break;
	default:
		break;
	}
	return NULL;
}

void ULostArcCharacterStatComponent::SetCurrentAttributeValue(EAttributeType Type, float Value)
{
	switch (Type)
	{
	case HP:
		CurrentHP = Value;
		OnProgressBarChanged.Broadcast(Type);
		
		if (CurrentHP < KINDA_SMALL_NUMBER)
		{
			CurrentHP = 0.0f;
			OnHPIsZero.Broadcast();
		}
		
		break;
	case MP:
		CurrentMP = Value;
		OnProgressBarChanged.Broadcast(Type);
		break;
	case ATK:
		CurrentATK = CurrentStatData->Attack + BonusATK;
		break;
	case DEF:
		CurrentDEF = CurrentStatData->Defense + BonusDEF;
		break;
	case CRT:
		CurrentCritical = CurrentStatData->Critical + BonusCritical;
	default:
		break;
	}
}

void ULostArcCharacterStatComponent::AddBonusAttribute(EAttributeType Type, float Value)
{
	switch (Type)
	{
	case HP:
		BonusMaxHP += Value;
		UE_LOG(LogTemp, Warning, TEXT("Add Bonous MaxHP(%d) : %f"), Type, BonusMaxHP);
		break;
	case MP:
		BonusMaxMP += Value;
		UE_LOG(LogTemp, Warning, TEXT("Add Bonous MaxMP(%d) : %f"), Type, BonusMaxMP);
		break;
	case ATK:
		BonusATK += Value;
		UE_LOG(LogTemp, Warning, TEXT("Add Bonous ATK(%d) : %f"), Type, BonusATK);
		break;
	case DEF:
		BonusDEF += Value; 
		UE_LOG(LogTemp, Warning, TEXT("Add Bonous DEF(%d) : %f"), Type, BonusDEF);
		break;
	case CRT:
		BonusCritical += Value;
		UE_LOG(LogTemp, Warning, TEXT("Add Bonous CRT(%d) : %f"), Type, BonusCritical);
	default:
		break;
	}
	
	SetCurrentAttributeValue(Type, GetCurrentAttributeValue(Type));
}

void ULostArcCharacterStatComponent::SetDamage(float NewDamage)
{
	SetCurrentAttributeValue(EAttributeType::HP, FMath::Clamp<float>(CurrentHP - FMath::Clamp<float>(NewDamage - (GetCurrentAttributeValue(EAttributeType::DEF)),1.f,NewDamage), 0.0f, CurrentHP));
}

void ULostArcCharacterStatComponent::ManaRegenerationPerSecond(float Amount)
{
	SetCurrentAttributeValue(EAttributeType::MP, FMath::Clamp<float>(CurrentMP + Amount, 0, GetMaxAttributeValue(EAttributeType::MP)));
}