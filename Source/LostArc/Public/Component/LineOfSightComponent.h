// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Components/SceneComponent.h"
#include "LineOfSightComponent.generated.h"

struct FSightTraceInfo
{
	bool Hit;
	FVector Point;
	float Dst;
	float Angle;
	FSightTraceInfo() {}
	FSightTraceInfo(bool InHit, FVector InPoint, float InDst, float InAngle)
		:Hit(InHit),
		Point(InPoint),
		Dst(InDst),
		Angle(InAngle)
	{}
};
class UMaterialInterface;
/**
 * 
 */
UCLASS(ClassGroup = (LineOfSight), meta = (BlueprintSpawnableComponent))
class LOSTARC_API ULineOfSightComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	/** 디버그 유무  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineOfSight")
	bool bDebugIsEnabled;
	/** 시야범위  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineOfSight")
	float SightRadius;
	/** 시야각도 ( 0 ~ 360 )  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineOfSight", meta = (UIMax = 360, UIMin = 0))
	float SightAngle;
	/** 장애물 타겟 채널  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineOfSight")
	TEnumAsByte<ETraceTypeQuery> ObstacleTraceType;
	/** 생성 Mesh 퀄리티 ( 버텍스 갯수 )  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineOfSight|MeshInfo")
	float MeshQuality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineOfSight|MeshInfo")
	UMaterialInterface* Material;

public:
	ULineOfSightComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector> normals;
	UPROPERTY()
	TArray<FVector2D> UV0;
	UPROPERTY()
	TArray<FLinearColor> vertexColors;
	UPROPERTY()
	TArray<FProcMeshTangent> tangents;
	void DrawLineOfSight();
	FSightTraceInfo GetSightTraceInfoInfo(float Angle);
	FVector DirectionFromAngle(float AngleInDegrees);
};
