// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "SaveLoadWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API USaveLoadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* SaveTemplateBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadTemplateBtn;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveMenu;

	UPROPERTY(meta = (BindWidget))
	UBorder* LoadMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseSaveMenuBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseLoadMenuBtn;

	UPROPERTY(meta = (BindWidget))
	UEditableText* SlotNameTxt;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SlotListScrollBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* ConfirmationBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ReplaceBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AlreadyExistsMsgTxt;

};
