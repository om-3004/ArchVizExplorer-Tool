// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SlotListWidget.h"

void USlotListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadSelectedSlotBtn->OnClicked.AddDynamic(this, &USlotListWidget::AfterSlotSelected);
	DeleteSelectedSlotBtn->OnClicked.AddDynamic(this, &USlotListWidget::AfterSlotDeleteBtnPressed);
	EditSelectedSlotNameBtn->OnClicked.AddDynamic(this, &USlotListWidget::AfterSlotNameEditBtnPressed);
}

void USlotListWidget::AfterSlotSelected()
{
	OnTemplateSlotPressed.ExecuteIfBound(LoadSlotName->GetText());
}

void USlotListWidget::AfterSlotDeleteBtnPressed()
{
	OnDeleteTemplateSlotPressed.ExecuteIfBound(LoadSlotName->GetText());
}

void USlotListWidget::AfterSlotNameEditBtnPressed()
{
	OnEditTemplateSlotNamePressed.ExecuteIfBound(LoadSlotName->GetText());
}
