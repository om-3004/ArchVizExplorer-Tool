// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteriorDataAsset.generated.h"

UENUM()
enum class EBuildingAsset {
	Wall,
	Floor,
	Roof
};

USTRUCT(BlueprintType)
struct FInteriorDesignData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "InteriorDesignData")
	EBuildingAsset BuildingAsset;

	UPROPERTY(EditAnywhere, Category = "InteriorDesignData")
	UTexture2D* InteriorImage;

	UPROPERTY(EditAnywhere, Category = "InteriorDesignData")
	UStaticMesh* InteriorStaticMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UInteriorDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "InteriorDesignData")
	TArray<FInteriorDesignData> InteriorDesignDataArray;
};
