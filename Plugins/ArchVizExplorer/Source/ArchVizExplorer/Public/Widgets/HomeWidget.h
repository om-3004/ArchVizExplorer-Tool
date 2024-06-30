// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

#include "HomeWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UHomeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeSelectionDropdown;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveTemplateBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadTemplateBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseBtn;

	UPROPERTY(meta = (BindWidget))
	UEditableText* SlotNameTxt;

};
