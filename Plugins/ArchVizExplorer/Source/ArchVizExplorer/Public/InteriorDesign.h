// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorDesign.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AInteriorDesign : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteriorDesign();

	UFUNCTION()
	void SetInteriorStaticMesh(UStaticMesh* StaticMesh);

	UPROPERTY()
	UStaticMesh* InteriorStaticMesh;

	UPROPERTY()
	EBuildingAsset BuildingAssetForInterior;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	USceneComponent* InteriorSceneComponent;

	UPROPERTY()
	UStaticMeshComponent* InteriorStaticMeshComponent;

};
