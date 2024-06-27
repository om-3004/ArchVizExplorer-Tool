// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/Layout/SScrollBox.h"
#include "DataAssets/WallDataAsset.h"
#include "DataAssets/DoorDataAsset.h"
#include "DataAssets/RoadMaterialDataAsset.h"
#include "DataAssets/BuildingMaterialDataAsset.h"

#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnWallSelection, const FWallData&)
DECLARE_DELEGATE_OneParam(FOnDoorSelection, const FDoorData&)

DECLARE_DELEGATE_OneParam(FOnRoadMaterialSelection, const FRoadMaterialData&)
DECLARE_DELEGATE_OneParam(FOnBuildingMaterialSelection, const FBuildingMaterialData&)

UENUM()
enum class EScrollBoxType : uint8 {
	WallScrollBox,
	DoorScrollBox,
	RoadMaterialScrollBox,
	BuildingMaterialScrollBox,
};

class ARCHVIZEXPLORER_API SScrollBoxSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScrollBoxSlate)
	{}
		SLATE_ARGUMENT(TWeakObjectPtr<UWallDataAsset>, InWallAssetManager)
		SLATE_ARGUMENT(TWeakObjectPtr<UDoorDataAsset>, InDoorAssetManager)

		SLATE_ARGUMENT(TWeakObjectPtr<URoadMaterialDataAsset>, InRoadMaterialAssetManager)
		SLATE_ARGUMENT(TWeakObjectPtr<UBuildingMaterialDataAsset>, InBuildingMaterialAssetManager)

		SLATE_ARGUMENT(EScrollBoxType, InScrollBoxType)
		SLATE_ARGUMENT(float, InThumbnailSizeScale)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FOnWallSelection OnWallSelection;
	FOnDoorSelection OnDoorSelection;
	FOnRoadMaterialSelection OnRoadMaterialSelection;
	FOnBuildingMaterialSelection OnBuildingMaterialSelection;

	TWeakObjectPtr<UWallDataAsset> WallDataAsset;
	TWeakObjectPtr<UDoorDataAsset> DoorDataAsset;
	TWeakObjectPtr<URoadMaterialDataAsset> RoadMaterialDataAsset;
	TWeakObjectPtr<UBuildingMaterialDataAsset> BuildingMaterialDataAsset;

	EScrollBoxType ScrollBoxType;

	UPROPERTY(EditAnywhere, Category = "SlateScrollBox", meta = (ClampMin = 1.0f, UIMin = 1.0f))
	float ThumbnailSizeScale{ 1.0f };

	void ScrollBoxSelection();

private:
	TSharedPtr<SBorder> RootBorder;
	TSharedPtr<STextBlock> RootBoxName;
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> RootVerticalBox;

	void PopulateWallScrollBox();
	void PopulateDoorScrollBox();
	void PopulateRoadMaterialScrollBox();
	void PopulateBuildingMaterialScrollBox();

	TWeakObjectPtr<UWallDataAsset> WallDataAssetPtr;
	TWeakObjectPtr<UDoorDataAsset> DoorDataAssetPtr;
	TWeakObjectPtr<URoadMaterialDataAsset> RoadMaterialDataAssetPtr;
	TWeakObjectPtr<UBuildingMaterialDataAsset> BuildingMaterialDataAssetPtr;

};
