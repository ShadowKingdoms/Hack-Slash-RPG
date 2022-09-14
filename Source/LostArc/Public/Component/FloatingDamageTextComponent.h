// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingDamageTextComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOSTARC_API UFloatingDamageTextComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFloatingDamageTextComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FloatingDamageText", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> FloatingTextActorClass = nullptr;

	// space between each number
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FloatingDamageText", meta=(AllowPrivateAccess = "true"))
	float TextVerticalSpacing = 40.0f;

	// verticalOffset a new text
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FloatingDamageText", meta=(AllowPrivateAccess = "true"))
	float TextVerticalOffset = 100.0f;

public:
	// 월드 공간의 지정된 위치에 지정된 텍스트를 추가합니다.
	UFUNCTION(Unreliable, BlueprintCallable, Category="FloatingDamageText")
	void AddFloatingText(const FText& text, FVector world_location);

private:
	// callback text actor is destroyed
	UFUNCTION()
	void OnTextDestroyed(AActor* destroyed_actor);

	// an array of all active text actors
	UPROPERTY()
	TArray<AActor*> ActivityTextActors;
};
