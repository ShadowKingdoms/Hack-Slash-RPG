// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/LostArcPlayerCharacter.h"
#include "Component/LostArcCharacterStatComponent.h"
#include "LostArcAbilityBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityCDDelegate, bool);

UCLASS(Abstract)
class LOSTARC_API ULostArcAbilityBase : public UObject
{
	GENERATED_BODY()

public:
	TPair<FTimerHandle, FOnAbilityCDDelegate> AbilityCDProperty;
	
	UPROPERTY()
	UActorComponent* OwnerComponent;
	
public:
	virtual bool Use(ALostArcPlayerCharacter* Character) PURE_VIRTUAL(ULostArcAbilityBase::Use, return false;);
	virtual bool AbilityStateCheck(ALostArcPlayerCharacter* Character) PURE_VIRTUAL(ULostArcAbilityBase::AbilityStateCheck, return false;);
	
	UTexture2D* GetAbility_Icon() { return Ability_Icon; }
	float GetCDTime() { return CoolDown; }
	FString GetName() { return Name; }

protected:
	UPROPERTY()
	UTexture2D* Ability_Icon;

	float CoolDown;
	FString Name;
};
