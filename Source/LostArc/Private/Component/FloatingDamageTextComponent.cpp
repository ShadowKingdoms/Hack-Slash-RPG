// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/FloatingDamageTextComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/UserInterfaceSettings.h"
#include "UI/Actors/FloatingTextActor.h"
#include "Kismet/GameplayStatics.h"

UFloatingDamageTextComponent::UFloatingDamageTextComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UFloatingDamageTextComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UFloatingDamageTextComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get the viewport size
	const FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	// get the viewport scale
	const int32 viewportX = FGenericPlatformMath::FloorToInt(viewportSize.X);
	const int32 viewportY = FGenericPlatformMath::FloorToInt(viewportSize.Y);
	const float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportX,viewportY));
	
	// get the owing player's camera location

	const FVector cameraLocation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->GetCameraLocation();

	// 뷰포트 크기와 스케일로 활성 텍스트 액터의 수직 위치를 화면에 균등하게 쌓는 것처럼 보입니다.
	for(int32 i = 1; i < ActivityTextActors.Num(); i++)
	{
		// 플로팅 텍스트 액터에서 카메라가 얼마나 멀리 떨어져 있는지에 따라 수직 오프셋을 계산합니다.
		AFloatingTextActor* textActor = ActivityTextActors[i];
		const float distance = FVector::Dist(cameraLocation, textActor->GetAnchorLocation());
		const float verticalOffset = distance / (viewportSize.X / viewportScale);

		// 플로팅 텍스트 액터의 위치를 계산된 양으로 조정합니다.
		textActor->SetActorLocation(textActor->GetAnchorLocation() + FVector(0.f,0.f, i * verticalOffset * TextVerticalSpacing));
	}
		
}

void UFloatingDamageTextComponent::AddFloatingText(const FText& text, FVector world_location)
{
	if(FloatingTextActorClass == nullptr)
		return;

	// 지정된 위치에 텍스트 액터 생성
	const FTransform spawnTransform = FTransform(FRotator::ZeroRotator, world_location + FVector(0.f, 0.f, TextVerticalOffset));
	AFloatingTextActor * newTextActor = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FloatingTextActorClass, spawnTransform, GetOwner());

	if(newTextActor == nullptr)
		return;

	// Initialize and finish spawning the actor
	newTextActor->Initialize(text);
	newTextActor->OnDestroyed.AddDynamic(this, &UFloatingDamageTextComponent::OnTextDestroyed);
	UGameplayStatics::FinishSpawningActor(newTextActor, spawnTransform); // 액터 생성을 마무리시키기 위해 호출되는 명시적인 호출문이며 ExecuteConstruction로 건너띕니다.

	// 배열의 시작 부분에 새 텍스트 액터 추가
	ActivityTextActors.Insert(newTextActor, 0);
}

void UFloatingDamageTextComponent::OnTextDestroyed(AActor* destroyed_actor)
{
	ActivityTextActors.Pop();
}