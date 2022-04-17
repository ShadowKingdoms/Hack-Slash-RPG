// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/RangedMonsterAIController.h"

ARangedMonsterAIController::ARangedMonsterAIController() : AMonsterBaseAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/Monster/BT_RangedMonster.BT_RangedMonster'")); // BP의 BT와 연동
	if (BTObject.Succeeded()) { BTAsset = BTObject.Object; }
}