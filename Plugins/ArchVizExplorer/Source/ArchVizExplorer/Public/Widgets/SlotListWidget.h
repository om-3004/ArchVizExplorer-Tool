// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "SlotListWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnTemplateSlotPressed, const FText&)
DECLARE_DELEGATE_OneParam(FOnDeleteTemplateSlotPressed, const FText&)

UCLASS()
class ARCHVIZEXPLORER_API USlotListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* LoadSelectedSlotBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadSlotName;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteSelectedSlotBtn;

	FOnTemplateSlotPressed OnTemplateSlotPressed;
	FOnDeleteTemplateSlotPressed OnDeleteTemplateSlotPressed;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void AfterSlotSelected();

	UFUNCTION()
	void AfterSlotDeleteBtnPressed();
};
