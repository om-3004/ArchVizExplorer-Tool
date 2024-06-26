// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "RoofGenerator.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoofGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoofGenerator();

	UPROPERTY()
	FVector RoofMeasurements{};

	UFUNCTION()
	void GenerateRoof(const FVector& Dimensions);

	UFUNCTION()
	void GenerateCube(const FVector& Dimensions, const FVector& LocationOffset);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	UProceduralMeshComponent* RoofProceduralMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "RoofGenerator | Material")
	UMaterialInterface* RoofProceduralMeshMaterial;

};
