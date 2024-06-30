// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FloorGenerator.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AFloorGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorGenerator();

	UFUNCTION()
	void GenerateFloor(const FVector& Dimensions);

	UFUNCTION()
	void ApplyMaterialToFloorActor(UMaterialInterface* Material);

	UPROPERTY()
	FVector FloorMeasurements{};

	UPROPERTY()
	UProceduralMeshComponent* FloorProceduralMeshComponent;

	UPROPERTY()
	UMaterialInterface* FloorMaterial;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FloorGenerator | Material")
	UMaterialInterface* FloorProceduralMeshMaterial;

};