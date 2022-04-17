// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controller/LostArcPlayerController.h"
#include "Abilities/Skill/LostArcSkillBase.h"
#include "UI/LostArcUIMainHUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/BossMonsterCharacter.h"
#include "Character/LostArcPlayerCharacter.h"

ALostArcPlayerController::ALostArcPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	static ConstructorHelpers::FClassFinder<ULostArcUIMainHUD> UI_HUD_C(TEXT("WidgetBlueprint'/Game/UI/HUD/BP_MainHUD.BP_MainHUD_C'"));

	if (UI_HUD_C.Succeeded())
	{
		MainHUDClass = UI_HUD_C.Class;
	}
}

void ALostArcPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ALostArcPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ALostArcPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("WheelUp", IE_Pressed, this, &ALostArcPlayerController::MouseWheelUp);
	InputComponent->BindAction("WheelDown", IE_Pressed, this, &ALostArcPlayerController::MouseWheelDown);
}

void ALostArcPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	MainHUD = CreateWidget<ULostArcUIMainHUD>(this, MainHUDClass);
	MainHUD->AddToViewport();
	
	InputComponent->BindAction<FBindWidgetDelegate>("Inventory", IE_Pressed, MainHUD, &ULostArcUIMainHUD::EnableWidget, ESlotType::Inven);
	InputComponent->BindAction<FBindWidgetDelegate>("Equipment", IE_Pressed, MainHUD, &ULostArcUIMainHUD::EnableWidget, ESlotType::Equip);
}

void ALostArcPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALostArcPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
	// Camera Situation Update
	if (bCameraSit.Key) 
	{
		CameraPositionChange(bCameraSit.Value);
	}
}

void ALostArcPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	if (!ULostArcSkillBase::bAnimationRunning)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ALostArcPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 100.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation); // Requires placement of NevmeshboundsVolume in level
		}
	}
}

void ALostArcPlayerController::OnSetDestinationPressed()
{
	if(!bOnSlotClick)
	{
		auto Char = Cast<ALostArcPlayerCharacter>(GetCharacter());
		Char->AbilityComponent->AbilityCancel();
		bMoveToMouseCursor = true;
	}
}

void ALostArcPlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
	bOnSlotClick = false;
}

void ALostArcPlayerController::MouseWheelUp()
{
	bCameraSit.Key = true;
	bCameraSit.Value = true;
	GetWorldTimerManager().ClearTimer(CameraZoomTimer);
	GetWorldTimerManager().SetTimer(CameraZoomTimer, FTimerDelegate::CreateLambda([&]() { bCameraSit.Key = false; }), 3.5f, false);
}

void ALostArcPlayerController::MouseWheelDown()
{
	bCameraSit.Key = true;
	bCameraSit.Value = false;
	GetWorldTimerManager().ClearTimer(CameraZoomTimer);
	GetWorldTimerManager().SetTimer(CameraZoomTimer, FTimerDelegate::CreateLambda([&]() { bCameraSit.Key = false; }), 3.5f, false);
}

void ALostArcPlayerController::CameraPositionChange(bool bWheel)
{
	auto ArcCharacter = Cast<ALostArcPlayerCharacter>(GetPawn());
	if (ArcCharacter == nullptr) return;
	if (bWheel) // Zomm in
	{
		ArcCharacter->GetCameraBoom()->TargetArmLength = FMath::Clamp(ArcCharacter->GetCameraBoom()->TargetArmLength -= 7.f, 300.f, 1250.f);
		ArcCharacter->GetCameraBoom()->SetRelativeRotation(FMath::Lerp(FQuat(ArcCharacter->GetCameraBoom()->GetRelativeRotation()), FQuat(FRotator(-20.0f, 0.0f, 0.0f)), 0.03));
		ArcCharacter->GetTopDownCameraComponent()->SetFieldOfView(FMath::Clamp(ArcCharacter->GetTopDownCameraComponent()->FieldOfView += .1f, 90.f, 100.f));
	}
	else // Zoom out
	{
		ArcCharacter->GetCameraBoom()->TargetArmLength = FMath::Clamp(ArcCharacter->GetCameraBoom()->TargetArmLength += 7.f, 300.f, 1250.f);
		ArcCharacter->GetCameraBoom()->SetRelativeRotation(FMath::Lerp(FQuat(ArcCharacter->GetCameraBoom()->GetRelativeRotation()), FQuat(FRotator(-65.0f, 0.0f, 0.0f)), 0.03));
		ArcCharacter->GetTopDownCameraComponent()->SetFieldOfView(FMath::Clamp(ArcCharacter->GetTopDownCameraComponent()->FieldOfView -= .1f, 90.f, 100.f));
	}
}