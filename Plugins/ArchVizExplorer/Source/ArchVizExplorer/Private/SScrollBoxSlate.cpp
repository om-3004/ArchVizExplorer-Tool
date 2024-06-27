// Fill out your copyright notice in the Description page of Project Settings.


#include "SScrollBoxSlate.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SScrollBoxSlate::Construct(const FArguments& InArgs)
{
	WallDataAssetPtr = InArgs._InWallAssetManager;
	DoorDataAssetPtr = InArgs._InDoorAssetManager;
	RoadMaterialDataAssetPtr = InArgs._InRoadMaterialAssetManager;
	BuildingMaterialDataAssetPtr = InArgs._InBuildingMaterialAssetManager;
	ScrollBoxType = InArgs._InScrollBoxType;
	ThumbnailSizeScale = InArgs._InThumbnailSizeScale;

	RootBorder = SNew(SBorder);
	FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.1843137254901961f, 0.23529411764705882f, 0.49411764705882355f, 1.0f));
	ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;

	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{ 5.f, 5.f, 5.f, 5.f };
	OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ColorBrush->OutlineSettings = OutlineSettings;
	RootBorder->SetBorderImage(ColorBrush);

	RootVerticalBox = SNew(SVerticalBox);
	RootBoxName = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FColor::White);
	ScrollBox = SNew(SScrollBox).Orientation(Orient_Horizontal);

	ScrollBoxSelection();

	RootVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
		[
			RootBoxName.ToSharedRef()
		];

	RootVerticalBox->AddSlot().HAlign(HAlign_Center)
		[
			ScrollBox.ToSharedRef()
		];

	RootBorder->SetContent(RootVerticalBox.ToSharedRef());

	ChildSlot
		[
			RootBorder.ToSharedRef()
		];
}

void SScrollBoxSlate::ScrollBoxSelection()
{
	switch (ScrollBoxType)
	{
	case EScrollBoxType::WallScrollBox:
		PopulateWallScrollBox();
		break;
	case EScrollBoxType::DoorScrollBox:
		PopulateDoorScrollBox();
		break;
	case EScrollBoxType::RoadMaterialScrollBox:
		PopulateRoadMaterialScrollBox();
		break;
	case EScrollBoxType::BuildingMaterialScrollBox:
		PopulateBuildingMaterialScrollBox();
		break;
	}
}

void SScrollBoxSlate::PopulateWallScrollBox()
{
	RootBoxName->SetText(FText::FromString("Wall"));
	ScrollBox->ClearChildren();
	if (WallDataAssetPtr.IsValid())
	{
		for (const FWallData& WallData : WallDataAssetPtr->WallTypes)
		{
			if (WallData.Image)
			{
				TSharedPtr<STextBlock> Name = SNew(STextBlock).Text(WallData.Name).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(WallData.Image);
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &WallData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							OnWallSelection.ExecuteIfBound(WallData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
					});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						Name.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

void SScrollBoxSlate::PopulateDoorScrollBox()
{
	RootBoxName->SetText(FText::FromString("Door"));
	ScrollBox->ClearChildren();
	if (WallDataAssetPtr.IsValid())
	{
		for (const FDoorData& DoorData : DoorDataAssetPtr->DoorTypes)
		{
			if (DoorData.Image)
			{
				TSharedPtr<STextBlock> Name = SNew(STextBlock).Text(DoorData.Name).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(DoorData.Image);
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &DoorData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnDoorSelection.ExecuteIfBound(DoorData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						Name.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

void SScrollBoxSlate::PopulateRoadMaterialScrollBox()
{
	RootBoxName->SetText(FText::FromString("Road Material"));
	ScrollBox->ClearChildren();
	if (RoadMaterialDataAssetPtr.IsValid())
	{
		for (const FRoadMaterialData& RoadMaterialData : RoadMaterialDataAssetPtr->RoadMaterialsArray)
		{
			if (RoadMaterialData.Image)
			{
				//TSharedPtr<STextBlock> Name = SNew(STextBlock).Text(RoadMaterialData.Name).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(RoadMaterialData.Image);
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &RoadMaterialData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnRoadMaterialSelection.ExecuteIfBound(RoadMaterialData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				/*ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						Name.ToSharedRef()
					];*/
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

void SScrollBoxSlate::PopulateBuildingMaterialScrollBox()
{
	RootBoxName->SetText(FText::FromString("Building Material"));
	ScrollBox->ClearChildren();
	if (RoadMaterialDataAssetPtr.IsValid())
	{
		for (const FBuildingMaterialData& BuildingMaterialData : BuildingMaterialDataAssetPtr->BuildingMaterialsArray)
		{
			if (BuildingMaterialData.Image)
			{
				//TSharedPtr<STextBlock> Name = SNew(STextBlock).Text(RoadMaterialData.Name).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(BuildingMaterialData.Image);
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &BuildingMaterialData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnBuildingMaterialSelection.ExecuteIfBound(BuildingMaterialData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				/*ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						Name.ToSharedRef()
					];*/
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION