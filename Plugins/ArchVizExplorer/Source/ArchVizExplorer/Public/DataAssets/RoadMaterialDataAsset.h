// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoadMaterialDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FRoadMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "RoadMaterialData")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, Category = "RoadMaterialData")
	UMaterialInterface* Material;
};

UCLASS()
class ARCHVIZEXPLORER_API URoadMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "RoadMaterialData")
	TArray<FRoadMaterialData> RoadMaterialsArray;
};
