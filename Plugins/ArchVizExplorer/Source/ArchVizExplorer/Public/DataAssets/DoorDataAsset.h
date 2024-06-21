// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DoorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FDoorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "DoorData")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "DoorData")
	UTexture* Image;

	UPROPERTY(EditAnywhere, Category = "DoorData")
	UStaticMesh* StaticMesh;

};

UCLASS()
class ARCHVIZEXPLORER_API UDoorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoorData")
	TArray<FDoorData> DoorTypes;

};
