// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "../SScrollBoxSlate.h"
#include "ScrollBoxWidget.generated.h"

DECLARE_DELEGATE_OneParam(FAfterWallSelection, const FWallData&)
DECLARE_DELEGATE_OneParam(FAfterDoorSelection, const FDoorData&)

DECLARE_DELEGATE_OneParam(FAfterRoadMaterialSelection, const FRoadMaterialData&)
DECLARE_DELEGATE_OneParam(FAfterBuildingMaterialSelection, const FBuildingMaterialData&)

UCLASS()
class ARCHVIZEXPLORER_API UScrollBoxWidget : public UWidget
{
	GENERATED_BODY()

public:
	
	FAfterWallSelection AfterWallSelection;
	FAfterDoorSelection AfterDoorSelection;
	FAfterRoadMaterialSelection AfterRoadMaterialSelection;
	FAfterBuildingMaterialSelection AfterBuildingMaterialSelection;

protected:
	virtual TSharedRef<SWidget>RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual const FText GetPaletteCategory() override;
	virtual void SynchronizeProperties() override;

private:
	TSharedPtr<SScrollBoxSlate> ScrollBoxAsset;

	UPROPERTY(EditAnywhere, Category = "ScrollBoxWidget")
	UWallDataAsset* WallDataAsset;
	
	UPROPERTY(EditAnywhere, Category = "ScrollBoxWidget")
	UDoorDataAsset* DoorDataAsset;

	UPROPERTY(EditAnywhere, Category = "ScrollBoxWidget")
	URoadMaterialDataAsset* RoadMaterialDataAsset;
	
	UPROPERTY(EditAnywhere, Category = "ScrollBoxWidget")
	UBuildingMaterialDataAsset* BuildingMaterialDataAsset;

	UPROPERTY(EditAnywhere, Category = "ScrollBoxWidget")
	EScrollBoxType ScrollBoxType;

	UPROPERTY(EditAnywhere, Category = "ScrollBoxWidget")
	float ThumbnailSizeScale{ 1.0f };

	UFUNCTION()
	void PassWallInController(const FWallData& WallData);
	
	UFUNCTION()
	void PassDoorInController(const FDoorData& DoorData);

	UFUNCTION()
	void PassRoadMaterialInController(const FRoadMaterialData& RoadMaterialData);
	
	UFUNCTION()
	void PassBuildingMaterialInController(const FBuildingMaterialData& BuildingMaterialData);
};
