// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LostArcPlayerCharacter.generated.h"

enum EAbilityType;

DECLARE_DELEGATE_OneParam(FBindAbilityDelegate, EAbilityType)
DECLARE_DELEGATE_OneParam(FBindQuickSlotDelegate, int32 Slot)

UCLASS(Blueprintable)
class LOSTARC_API ALostArcPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALostArcPlayerCharacter();
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class ULostArcCharacterStatComponent* StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class ULostArcCharacterAbilityComponent* AbilityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ULostArcCharacterAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class ULostArcQuickSlotComponent* QuickSlotComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class ULostArcInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class ULostArcCharacterEquipComponent* EquipComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
