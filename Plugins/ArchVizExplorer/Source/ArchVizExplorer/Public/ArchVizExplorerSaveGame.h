// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WallGenerator.h"
#include "DataAssets/InteriorDataAsset.h"
#include "ArchVizExplorerSaveGame.generated.h"

USTRUCT()
struct FRoadSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform RoadTransform;

	UPROPERTY()
	UMaterialInterface* RoadMaterial;

	UPROPERTY()
	FVector RoadDimensions;
};

USTRUCT()
struct FWallSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform WallTransform;

	UPROPERTY()
	UMaterialInterface* WallMaterial;

	UPROPERTY()
	int32 NoOfSegments;

	UPROPERTY()
	TMap<int32, FPair> WallActorMap;
};

USTRUCT()
struct FFloorSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform FloorTransform;

	UPROPERTY()
	UMaterialInterface* FloorMaterial;

	UPROPERTY()
	FVector FloorDimensions;
};

USTRUCT()
struct FRoofSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform RoofTransform;

	UPROPERTY()
	UMaterialInterface* RoofMaterial;

	UPROPERTY()
	FVector RoofDimensions;
};

USTRUCT()
struct FInteriorSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform InteriorTransform;

	UPROPERTY()
	EBuildingAsset BuildingAssetForInterior;

	UPROPERTY()
	UStaticMesh* InteriorMesh;
};


UCLASS()
class ARCHVIZEXPLORER_API UArchVizExplorerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TArray<FRoadSaveData> RoadActorArray;
	
	UPROPERTY()
	TArray<FWallSaveData> WallActorArray;
	
	UPROPERTY()
	TArray<FFloorSaveData> FloorActorArray;
	
	UPROPERTY()
	TArray<FRoofSaveData> RoofActorArray;
	
	UPROPERTY()
	TArray<FInteriorSaveData> InteriorActorArray;
};
