// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Widgets/Layout/SScrollBox.h"
#include "DataAssets/WallDataAsset.h"
#include "DataAssets/DoorDataAsset.h"

#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnWallSelection, const FWallData&)
DECLARE_DELEGATE_OneParam(FOnDoorSelection, const FDoorData&)

UENUM()
enum class EScrollBoxType : uint8 {
	WallScrollBox,
	DoorScrollBox,
};

class ARCHVIZEXPLORER_API SScrollBoxSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScrollBoxSlate)
	{}
		SLATE_ARGUMENT(TWeakObjectPtr<UWallDataAsset>, InWallAssetManager)
		SLATE_ARGUMENT(TWeakObjectPtr<UDoorDataAsset>, InDoorAssetManager)
		SLATE_ARGUMENT(EScrollBoxType, InScrollBoxType)
		SLATE_ARGUMENT(float, InThumbnailSizeScale)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FOnWallSelection OnWallSelection;
	FOnDoorSelection OnDoorSelection;

	TWeakObjectPtr<UWallDataAsset> WallDataAsset;
	TWeakObjectPtr<UDoorDataAsset> DoorDataAsset;
	EScrollBoxType ScrollBoxType;
	float ThumbnailSizeScale{ 1.0f };
	void ScrollBoxSelection();

private:
	TSharedPtr<SBorder> RootBorder;
	TSharedPtr<STextBlock> RootBoxName;
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> RootVerticalBox;

	void PopulateWallScrollBox();
	void PopulateDoorScrollBox();


	TWeakObjectPtr<UWallDataAsset> WallDataAssetPtr;
	TWeakObjectPtr<UDoorDataAsset> DoorDataAssetPtr;

	UPROPERTY(EditAnywhere, Category = "SlateScrollBox", meta = (ClampMin = 1.0f, UIMin = 1.0f))

};
