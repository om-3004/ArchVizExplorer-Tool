// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WallInteriors.generated.h"

USTRUCT(BlueprintType)
struct FWallInteriorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WallInteriorData")
	int8 Index;

	UPROPERTY(EditAnywhere, Category = "WallInteriorData")
	UTexture2D* InteriorImage;

	UPROPERTY(EditAnywhere, Category = "WallInteriorData")
	UStaticMesh* InteriorStaticMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UWallInteriors : public UDataAsset
{
	GENERATED_BODY()
	
public:
	TArray<FWallInteriorData> WallInteriorDataArray;

};
