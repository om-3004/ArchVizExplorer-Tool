// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/SpinBox.h"

#include "RoadConstructionWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API URoadConstructionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UButton* ModeToggleBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ModeToggleBtnText;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* WidthBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LocationBox;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthValue;

	UPROPERTY(meta = (BindWidget))
	USpinBox* LocationX;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* LocationY;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoadConstructionMsg;

	UPROPERTY(meta = (BindWidget))
	UButton* NewRoadBtn;
};
