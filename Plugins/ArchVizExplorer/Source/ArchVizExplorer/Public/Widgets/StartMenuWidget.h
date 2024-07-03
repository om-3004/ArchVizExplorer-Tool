// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Border.h"

#include "StartMenuWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* BlankProjectBtn;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* TemplateListComboBox;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadTemplateBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* LoadBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CloseLoadTemplateBtn;

	UPROPERTY(meta = (BindWidget))
	UBorder* LoadTemplateMenu;

};
