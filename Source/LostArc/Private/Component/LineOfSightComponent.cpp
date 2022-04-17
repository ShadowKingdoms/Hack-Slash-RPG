// Fill out your copyright notice in the Description page of Project Settings.  

#include "Component/LineOfSightComponent.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ULineOfSightComponent::ULineOfSightComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;

    SightRadius = 300.0f;
    SightAngle = 120.0f;
    MeshQuality = 5.0f;

    CastShadow = false;
}

void ULineOfSightComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Material)
    {
        SetMaterial(0, Material);
    }
}

void ULineOfSightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    DrawLineOfSight();
}

void ULineOfSightComponent::DrawLineOfSight()
{
    /**
     *    MeshQuality 값이 클수록 많이 폴리곤을 그리기 위하여 SightAngle값에 MeshQuality를 곱한 값을
     *    SightAngle값으로 나누어 최소 AngleSize 를 구한다.
     *    현재 컴포넌트의 Yaw(Z축) 값을 기준으로 좌측 부터 우측 까지 최소 AngleSize 만큼 더해가면서 장애물을 Trace 하여
     *  버텍스 위치값을 설정한다
     */

    int32 AnglePieceCount = FMath::RoundToInt(SightAngle * MeshQuality);
    float DivideAngleSize = SightAngle / AnglePieceCount;

    TArray<FVector> ViewPoints;

    for (int32 index = 0; index <= AnglePieceCount; ++index)
    {
        FRotator ViewRotation = GetComponentRotation();
        float Angle = ViewRotation.Yaw - (SightAngle / 2.0f) + (DivideAngleSize * index);
        FSightTraceInfo SightTraceInfo = GetSightTraceInfoInfo(Angle);
        ViewPoints.Add(SightTraceInfo.Point);
    }

    int32 VertextCount = ViewPoints.Num() + 1;
    Vertices.SetNum(VertextCount);

    int32 TriangleCount = (VertextCount - 2) * 3;
    Triangles.SetNum(TriangleCount);

    Vertices[0] = FVector::ZeroVector;
    for (int32 index = 0; index < VertextCount - 1; ++index)
    {
        //버텍스 월드좌표를 컴포넌트의 로컬좌표로 변경
        Vertices[index + 1] = GetComponentTransform().InverseTransformPosition(ViewPoints[index]);

        //인덱스 버퍼 설정
        if (index < VertextCount - 2)
        {
            Triangles[index * 3 + 2] = 0;
            Triangles[index * 3 + 1] = index + 1;
            Triangles[index * 3] = index + 2;
        }
    }

    CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, UV0, vertexColors, tangents, false);
}

FSightTraceInfo ULineOfSightComponent::GetSightTraceInfoInfo(float Angle)
{
    EDrawDebugTrace::Type DrawDebugType = bDebugIsEnabled ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    //Angle 값으로 x축 y축 방향을 구한다
    FVector Dir = DirectionFromAngle(Angle);
    FHitResult OutHit;
    FVector Start = GetComponentLocation();
    FVector End = (Dir * SightRadius) + Start;
    TArray<AActor*> ActorsToIgnore;

    if (UKismetSystemLibrary::LineTraceSingle(this, Start, End, ObstacleTraceType, false, ActorsToIgnore, DrawDebugType, OutHit, true))
    {
        return FSightTraceInfo(true, OutHit.ImpactPoint, OutHit.Distance, Angle);
    }
    else
    {
        return FSightTraceInfo(false, End, SightRadius, Angle);
    }
}

FVector ULineOfSightComponent::DirectionFromAngle(float AngleInDegrees)
{
    return FVector(FMath::Cos(FMath::DegreesToRadians(AngleInDegrees)), FMath::Sin(FMath::DegreesToRadians(AngleInDegrees)), 0.0f);
}