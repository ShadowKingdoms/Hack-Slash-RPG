// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Items/LostArcItemBase.h"
#include "Component/LostArcInventoryComponent.h"
#include "Component/LostArcCharacterEquipComponent.h"
#include "LostArcItemEquipBase.generated.h"

UCLASS()
class LOSTARC_API ULostArcItemEquipBase : public ULostArcItemBase
{
	GENERATED_BODY()
	
public:
	ULostArcItemEquipBase(const FObjectInitializer& ObjectInitializer);
	virtual bool Use(ALostArcPlayerCharacter* Character) override;
	virtual void Equipment(ALostArcPlayerCharacter* Character) {}
	virtual void Dismount(ALostArcPlayerCharacter* Character) {}
	
	EAccessoryType GetAcType() const { return AcType; }
	
protected:
	EAccessoryType AcType;
	float Value = 0.f;
};
