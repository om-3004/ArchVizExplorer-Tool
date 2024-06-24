// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"
#include "ScrollBoxWidget.h"
#include "Components/TextBlock.h"

#include "BuildingConstructionWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UBuildingConstructionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* WallBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* DoorBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* FloorBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* RoofBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* BuildingModeToggleBtn;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BuildingModeToggleBtnText;

	// Wall Generator
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SegmentBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* NoSegmentsValue;
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* WallScrollBoxWidget;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LocationBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* LocationX;
	UPROPERTY(meta = (BindWidget))
	USpinBox* LocationY;
	UPROPERTY(meta = (BindWidget))
	UButton* DestroyWallBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* UpdateWallLocationUnderCursorBtn;

	// Door Generator
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* DoorScrollBoxWidget;
	UPROPERTY(meta = (BindWidget))
	UButton* DestroyDoorBtn;

	// Floor Generator
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* FloorDimensions;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorDimensionX;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorDimensionY;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorDimensionZ;
};
