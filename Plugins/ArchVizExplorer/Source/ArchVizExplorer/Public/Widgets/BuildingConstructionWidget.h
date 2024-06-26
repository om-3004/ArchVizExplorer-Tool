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
	UHorizontalBox* FloorLocationBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorLocationX;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorLocationY;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* FloorDimensionsBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorDimensionX;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorDimensionY;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorDimensionZ;
	UPROPERTY(meta = (BindWidget))
	UButton* DestroyFloorBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* UpdateFloorLocationUnderCursorBtn;
	
	//Roof Construction
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* RoofLocationBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofLocationX;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofLocationY;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofLocationZ;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* RoofDimensionsBox;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofDimensionX;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofDimensionY;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofDimensionZ;
	UPROPERTY(meta = (BindWidget))
	UButton* DestroyRoofBtn;

	// Roof Animation
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoofErrorMsgTxt;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* RoofErrorMsgAnim;
};
