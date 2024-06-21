// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WallDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FWallData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WallData")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "WallData")
	UTexture* Image;

	UPROPERTY(EditAnywhere, Category = "WallData")
	UStaticMesh* StaticMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UWallDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "WallData")
	TArray<FWallData> WallTypes;
};
