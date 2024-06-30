// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ScrollBoxWidget.h"

TSharedRef<SWidget> UScrollBoxWidget::RebuildWidget()
{
	ScrollBoxAsset = SNew(SScrollBoxSlate).InWallAssetManager(WallDataAsset).InDoorAssetManager(DoorDataAsset).InRoadMaterialAssetManager(RoadMaterialDataAsset).InBuildingMaterialAssetManager(BuildingMaterialDataAsset).InInteriorDesignAssetManager(InteriorDesignDataAsset).InThumbnailSizeScale(ThumbnailSizeScale).InScrollBoxType(ScrollBoxType);

	if (ScrollBoxAsset.IsValid()) {
		ScrollBoxAsset->OnWallSelection.BindUFunction(this, "PassWallInController");
		ScrollBoxAsset->OnDoorSelection.BindUFunction(this, "PassDoorInController");
		ScrollBoxAsset->OnRoadMaterialSelection.BindUFunction(this, "PassRoadMaterialInController");
		ScrollBoxAsset->OnBuildingMaterialSelection.BindUFunction(this, "PassBuildingMaterialInController");
		ScrollBoxAsset->OnInteriorDesignSelection.BindUFunction(this, "PassInteriorDesignInController");
	}

	return ScrollBoxAsset.ToSharedRef();
}

void UScrollBoxWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ScrollBoxAsset.Reset();
}

const FText UScrollBoxWidget::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UScrollBoxWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (ScrollBoxAsset.IsValid()) {
		ScrollBoxAsset->DoorDataAsset = DoorDataAsset;
		ScrollBoxAsset->WallDataAsset = WallDataAsset;
		ScrollBoxAsset->RoadMaterialDataAsset = RoadMaterialDataAsset;
		ScrollBoxAsset->BuildingMaterialDataAsset = BuildingMaterialDataAsset;
		ScrollBoxAsset->InteriorDesignDataAsset = InteriorDesignDataAsset;
		ScrollBoxAsset->ScrollBoxType = ScrollBoxType;
		ScrollBoxAsset->ThumbnailSizeScale = ThumbnailSizeScale;
		ScrollBoxAsset->ScrollBoxSelection();
	}
}

void UScrollBoxWidget::PassWallInController(const FWallData& WallData)
{
	AfterWallSelection.ExecuteIfBound(WallData);
}
void UScrollBoxWidget::PassDoorInController(const FDoorData& DoorData)
{
	AfterDoorSelection.ExecuteIfBound(DoorData);
}

void UScrollBoxWidget::PassRoadMaterialInController(const FRoadMaterialData& RoadMaterialData)
{
	AfterRoadMaterialSelection.ExecuteIfBound(RoadMaterialData);
}
void UScrollBoxWidget::PassBuildingMaterialInController(const FBuildingMaterialData& BuildingMaterialData)
{
	AfterBuildingMaterialSelection.ExecuteIfBound(BuildingMaterialData);
}

void UScrollBoxWidget::PassInteriorDesignInController(const FInteriorDesignData& InteriorDesignData)
{
	AfterInteriorDesignSelection.ExecuteIfBound(InteriorDesignData);
}
