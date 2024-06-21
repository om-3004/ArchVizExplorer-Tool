// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ComboBoxString.h"

#include "HomeWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UHomeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeSelectionDropdown;

};
