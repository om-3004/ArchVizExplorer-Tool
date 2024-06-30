// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WallGenerator.generated.h"

USTRUCT()
struct FPair {
	GENERATED_BODY()

	UPROPERTY()
	UStaticMesh* StaticMesh;

	UPROPERTY()
	UProceduralMeshComponent* ProceduralMeshComponent;
};

UCLASS()
class ARCHVIZEXPLORER_API AWallGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWallGenerator();

	UFUNCTION()
	void GenerateWall(const int32& NoOfSegments);

	UFUNCTION()
	void GenerateCube(const FVector& Dimensions, const FVector& LocationOffset, UProceduralMeshComponent* CubeComponent);

	UFUNCTION()
	void ApplyMaterialToWallActor(UMaterialInterface* Material);

	UPROPERTY(EditDefaultsOnly, Category = "WallGenerator")
	UStaticMesh* WallStaticMesh;

	UPROPERTY()
	float HeightOfWall;

	UPROPERTY()
	TArray<UStaticMeshComponent*> WallStaticMeshComponentsArr;

	UPROPERTY()
	TMap<int32, FPair> WallActorMap;

	UPROPERTY()
	TArray<int32> Indices;

	UPROPERTY()
	int32 SegmentsNo;

	UPROPERTY()
	UMaterialInterface* WallMaterial;

	UFUNCTION()
	void DestroyComponents();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UStaticMeshComponent* WallStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "WallGenerator | Material")
	UMaterialInterface* WallProceduralMeshMaterial;

	UFUNCTION()
	void UpdateDoorsAndProceduralMeshComponent(int32 NoOfSegments);

	UFUNCTION()
	void CheckReducedSegments(const int32& NoOfSegments);
};

