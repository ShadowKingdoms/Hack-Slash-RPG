// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LostArcUITabs.generated.h"

UCLASS()
class LOSTARC_API ULostArcUITabs : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUITabsFromParent(UUserWidget* Parent);
	
	UPROPERTY()
	UUserWidget* ParentWidget;

private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override; 
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
