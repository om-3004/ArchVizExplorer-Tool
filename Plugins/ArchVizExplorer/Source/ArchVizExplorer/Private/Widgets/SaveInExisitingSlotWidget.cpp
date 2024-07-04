// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SaveInExisitingSlotWidget.h"

void USaveInExisitingSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadSelectedSlotBtn->OnClicked.AddDynamic(this, &USaveInExisitingSlotWidget::AfterSlotSelected);
}

void USaveInExisitingSlotWidget::AfterSlotSelected()
{
	OnProjectSlotPressed.ExecuteIfBound(LoadSlotName->GetText());
}
