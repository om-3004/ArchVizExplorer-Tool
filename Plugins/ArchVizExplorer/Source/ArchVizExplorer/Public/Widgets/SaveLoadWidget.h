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

	UPROPERTY(meta = (BindWidget))
	UBorder* RenameMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseRenameMenuBtn;

	UPROPERTY(meta = (BindWidget))
	UEditableText* NewSlotNameTxt;

	UPROPERTY(meta = (BindWidget))
	UButton* RenameBtn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RenamingSlotMsgTxt;

	UPROPERTY(meta = (BindWidget))
	UBorder* SlotTypeMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* NewSlotBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* ExisitingSlotBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CloseSlotTypeMenuBtn;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveInExisitingSlotMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseSaveInExistingSlotMenuBtn;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SaveInExisitingSlotListScrollBox;
};
