// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ScrollBoxWidget.h"

TSharedRef<SWidget> UScrollBoxWidget::RebuildWidget()
{
	ScrollBoxAsset = SNew(SScrollBoxSlate).InDoorAssetManager(DoorDataAsset).InWallAssetManager(WallDataAsset).InThumbnailSizeScale(ThumbnailSizeScale).InScrollBoxType(ScrollBoxType);

	if (ScrollBoxAsset.IsValid()) {
		ScrollBoxAsset->OnWallSelection.BindUFunction(this, "PassWallInController");
		ScrollBoxAsset->OnDoorSelection.BindUFunction(this, "PassDoorInController");
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
