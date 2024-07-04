// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SaveInExisitingSlotWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnProjectSlotPressed, const FText&)

UCLASS()
class ARCHVIZEXPLORER_API USaveInExisitingSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* LoadSelectedSlotBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LoadSlotName;

	FOnProjectSlotPressed OnProjectSlotPressed;


protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void AfterSlotSelected();
};
