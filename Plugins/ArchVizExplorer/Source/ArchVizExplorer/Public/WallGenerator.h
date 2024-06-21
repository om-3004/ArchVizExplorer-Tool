// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WallGenerator.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "WallGenerator")
	UStaticMesh* WallStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "WallGenerator")
	UMaterialInterface* ProceduralMeshMaterial;

	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	float HeightOfWall;

	UFUNCTION()
	void DestroyComponents();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	UStaticMeshComponent* WallStaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	TArray<UStaticMeshComponent*> WallStaticMeshComponentsArr;
};

