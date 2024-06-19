// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/SpinBox.h"
#include "Components/Button.h"

#include "BuildingConstructionWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UBuildingConstructionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* NoSegmentsValue;

	UPROPERTY(meta = (BindWidget))
	UButton* WallBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* DoorBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* FloorBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* RoofBtn;
};
