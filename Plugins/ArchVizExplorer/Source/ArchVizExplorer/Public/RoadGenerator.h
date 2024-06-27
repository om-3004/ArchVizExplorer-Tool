// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "RoadGenerator.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoadGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoadGenerator();

	UFUNCTION(BlueprintCallable, Category = "RoadGenerator")
	void GenerateCube(const FVector& Dimensions, const FVector& LocationOffset);

	UPROPERTY()
	UProceduralMeshComponent* RoadProceduralMeshComponent;

	UPROPERTY()
	FVector RoadMeasurements{};
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "RoadGenerator | Material")
	UMaterialInterface* RoadProceduralMeshMaterial;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadGenerator")
	TArray<FVector> Vertices;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadGenerator")
	TArray<int32> Triangles;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadGenerator")
	TArray<FVector> Normals;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadGenerator")
	TArray<FVector2D> UVs;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadGenerator")
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadGenerator")
	TArray<FLinearColor> Colors;

	UFUNCTION()
	int AddNewVertex(const FVector& VertexCordinates);

	UFUNCTION()
	int AddUV(const FVector2D& UVCordinates);

	UFUNCTION()
	void DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2);

};
