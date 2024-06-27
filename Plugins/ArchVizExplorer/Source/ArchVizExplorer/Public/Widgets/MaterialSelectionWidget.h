// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxWidget.h"
#include "MaterialSelectionWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UMaterialSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* RoadMaterialScrollBox;
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* BuildingMaterialScrollBox;
};
