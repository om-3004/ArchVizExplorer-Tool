// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"

AArchVizController::AArchVizController() : getLocation{ true }, isFirstClick{ true }, bIsInBuildingEditorMode{ false }, bShouldEditWallLocationUnderCursor{ false }, bShouldEditFloorLocationUnderCursor{ false }
{

}

void AArchVizController::PreviewWall() {
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, WallGeneratorActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		WallLocation = HitResult.Location;
		if (WallGeneratorActor) {
			WallGeneratorActor->SetActorLocation(WallLocation);
			if (WallGeneratorActor->WallStaticMesh) { SnapActor(WallGeneratorActor->WallStaticMesh->GetBounds().GetBox().GetSize().Y / 2); }
			if (CurrentBuildingMode == EBuildingMode::EditorMode) {
				BuildingConstructionWidget->LocationX->SetValue(WallGeneratorActor->GetActorLocation().X);
				BuildingConstructionWidget->LocationY->SetValue(WallGeneratorActor->GetActorLocation().Y);
			}
		}
	}
}
void AArchVizController::PreviewFloor()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, FloorGeneratorActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		FVector ResultedLocation = HitResult.Location;
		ResultedLocation.Z = 0;
		if (FloorGeneratorActor) {
			FloorGeneratorActor->SetActorLocation(ResultedLocation);
			SnapActor(12.5);
		}
	}
}

void AArchVizController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((CurrentSelectedMode == EModeSelected::BuildingConstruction)) {
		if ((CurrentBuildingMode == EBuildingMode::ConstructionMode && CurrentBuildingComponent == EBuildingComponent::Wall && WallGeneratorActor) || (bShouldEditWallLocationUnderCursor)) {
			PreviewWall();
		}
		else if (CurrentBuildingMode == EBuildingMode::EditorMode && bShouldEditFloorLocationUnderCursor) {
			PreviewFloor();
		}
	}
}

void AArchVizController::CreateWidgets() {
	if (HomeWidgetClassRef) {
		HomeWidget = CreateWidget<UHomeWidget>(this, HomeWidgetClassRef);
		HomeWidget->AddToViewport();
	}
	if (RoadConstructionWidgetClassRef) {
		RoadConstructionWidget = CreateWidget<URoadConstructionWidget>(this, RoadConstructionWidgetClassRef);
	}
	if (BuildingConstructionWidgetClassRef) {
		BuildingConstructionWidget = CreateWidget<UBuildingConstructionWidget>(this, BuildingConstructionWidgetClassRef);
	}
}
void AArchVizController::BindWidgetDelegates() {
	if (HomeWidget && HomeWidgetClassRef) {
		HomeWidget->ModeSelectionDropdown->OnSelectionChanged.AddDynamic(this, &AArchVizController::OnModeSelectionChanged);
	}
	if (RoadConstructionWidget && RoadConstructionWidgetClassRef) {
		RoadConstructionWidget->ModeToggleBtn->OnClicked.AddDynamic(this, &AArchVizController::OnRoadModeToggleBtnClicked);
		RoadConstructionWidget->WidthValue->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadWidthValueChanged);
		RoadConstructionWidget->LocationX->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadLocationXValueChanged);
		RoadConstructionWidget->LocationY->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadLocationYValueChanged);
		RoadConstructionWidget->NewRoadBtn->OnClicked.AddDynamic(this, &AArchVizController::GenerateNewRoad);
	}
	if (BuildingConstructionWidget && BuildingConstructionWidgetClassRef) {
		BuildingConstructionWidget->WallBtn->OnClicked.AddDynamic(this, &AArchVizController::OnWallBtnClicked);
		BuildingConstructionWidget->DoorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDoorBtnClicked);
		BuildingConstructionWidget->FloorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnFloorBtnClicked);
		BuildingConstructionWidget->RoofBtn->OnClicked.AddDynamic(this, &AArchVizController::OnRoofBtnClicked);
		BuildingConstructionWidget->BuildingModeToggleBtn->OnClicked.AddDynamic(this, &AArchVizController::OnBuildingModeToggleBtnClicked);

		// Wall Generator
		BuildingConstructionWidget->NoSegmentsValue->OnValueChanged.AddDynamic(this, &AArchVizController::OnSegmentsChanged);
		BuildingConstructionWidget->WallScrollBoxWidget->AfterWallSelection.BindUFunction(this, "SetWallStaticMesh");
		BuildingConstructionWidget->LocationX->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallLocationXValueChanged);
		BuildingConstructionWidget->LocationY->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallLocationYValueChanged);
		BuildingConstructionWidget->DestroyWallBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDestroyWallBtnClicked);
		BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnUpdateWallLocationUnderCursorBtnClicked);

		// Door Generator
		BuildingConstructionWidget->DoorScrollBoxWidget->AfterDoorSelection.BindUFunction(this, "SetDoorMesh");
		BuildingConstructionWidget->DestroyDoorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDestroyDoorBtnClicked);

		// Floor Generator
		BuildingConstructionWidget->FloorLocationX->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorLocationXValueChanged);
		BuildingConstructionWidget->FloorLocationY->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorLocationYValueChanged);
		BuildingConstructionWidget->FloorDimensionX->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorDimensionXValueChanged);
		BuildingConstructionWidget->FloorDimensionY->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorDimensionYValueChanged);
		BuildingConstructionWidget->FloorDimensionZ->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorDimensionZValueChanged);
		BuildingConstructionWidget->DestroyFloorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDestroyFloorBtnClicked);
		BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnUpdateFloorLocationUnderCursorBtnClicked);
	}
}
void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	bShowMouseCursor = true;

	CreateWidgets();
	BindWidgetDelegates();
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetupRoadConstructionInputs();
	SetupBuildingEditorInputs();
	SetupWallConstructionInputs();
	SetupDoorConstructionInputs();
	SetupFloorConstructionInputs();
}

// Home Widget Bind Function
void AArchVizController::UpdateWidget() {
	switch (CurrentSelectedMode) {
	case EModeSelected::ViewMode:
		if (RoadConstructionWidget->IsInViewport()) { RoadConstructionWidget->RemoveFromParent(); }
		if (BuildingConstructionWidget->IsInViewport()) { BuildingConstructionWidget->RemoveFromParent(); }
		break;
	case EModeSelected::RoadConstruction:
		if (BuildingConstructionWidget->IsInViewport()) { BuildingConstructionWidget->RemoveFromParent(); }
		if (RoadConstructionWidgetClassRef) { RoadConstructionWidget->AddToViewport(); }
		break;
	case EModeSelected::BuildingConstruction:
		if (RoadConstructionWidget->IsInViewport()) { RoadConstructionWidget->RemoveFromParent(); }
		if (BuildingConstructionWidgetClassRef) { BuildingConstructionWidget->AddToViewport(); }
		break;
	}
}
void AArchVizController::UpdateBuildingMappings() {
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->ClearAllMappings();

		if (CurrentBuildingMode == EBuildingMode::ConstructionMode) {
			switch (CurrentBuildingComponent) {
			case EBuildingComponent::None:
				break;
			case EBuildingComponent::Wall:
				Subsystem->AddMappingContext(WallConstructionIMC, 0);
				break;
			case EBuildingComponent::Door:
				Subsystem->AddMappingContext(DoorConstructionIMC, 0);
				break;
			case EBuildingComponent::Floor:
				Subsystem->AddMappingContext(FloorConstructionIMC, 0);
				break;
			case EBuildingComponent::Roof:
				break;
			}
		}
		else {
			Subsystem->AddMappingContext(BuildingEditorIMC, 0);
		}

	}
}
void AArchVizController::UpdateInputMappings() {
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->ClearAllMappings();

		switch (CurrentSelectedMode)
		{
		case EModeSelected::ViewMode:
			break;
		case EModeSelected::RoadConstruction:
			Subsystem->AddMappingContext(RoadConstructionIMC, 0);
			break;
		case EModeSelected::BuildingConstruction:
			UpdateBuildingMappings();
			break;
		}
	}
}
void AArchVizController::SetDefaultMode() {
	switch (CurrentSelectedMode) {
	case EModeSelected::RoadConstruction:
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Switch to Editor Mode"));
		CurrentRoadMode = ERodeMode::ConstructionMode;
		RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->RoadConstructionMsg->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Visible);
		break;
	case EModeSelected::BuildingConstruction:
		CurrentBuildingComponent = EBuildingComponent::None;
		CurrentBuildingMode = EBuildingMode::ConstructionMode;
		BuildingConstructionWidget->BuildingModeToggleBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->BuildingModeToggleBtnText->SetText(FText::FromString("Switch to Editor Mode"));
		BuildingConstructionWidget->WallBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->DoorBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->FloorBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->RoofBtn->SetVisibility(ESlateVisibility::Visible);

		// Wall Editor Widgets
		BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);

		// Door Editor Widgets
		BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);

		// Floor Editor Widgets
		BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		break;
	}

}
void AArchVizController::OnModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == FString("View Mode")) {
		CurrentSelectedMode = EModeSelected::ViewMode;
		//if(WallGeneratorActor){ WallGeneratorActor = nullptr; }
		DestroyWallGeneratorActor();

		UpdateWidget();
		UpdateInputMappings();
	}
	else if (SelectedItem == FString("Road Construction")) {
		CurrentSelectedMode = EModeSelected::RoadConstruction;
		SetDefaultMode();

		//if(WallGeneratorActor){ WallGeneratorActor = nullptr; }
		DestroyWallGeneratorActor();

		if (RoadConstructionWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
	}
	else if (SelectedItem == FString("Building Construction")) {
		CurrentSelectedMode = EModeSelected::BuildingConstruction;
		SetDefaultMode();

		DestroyWallGeneratorActor();

		if (BuildingConstructionWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
	}
}

// Road Construction
void AArchVizController::SetupRoadConstructionInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		RoadConstructionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforRoadConstruction = NewObject<UInputAction>(this);
		ClickforRoadConstruction->ValueType = EInputActionValueType::Boolean;
		RoadConstructionIMC->MapKey(ClickforRoadConstruction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforRoadConstruction, ETriggerEvent::Completed, this, &AArchVizController::GetRoadLocationOnClick);
	}
}
void AArchVizController::GetRoadLocationOnClick()
{
	if (CurrentRoadMode == ERodeMode::ConstructionMode) {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

		FVector CursorWorldLocation;
		FVector CursorWorldDirection;

		if (isFirstClick) {
			if (getLocation) {
				DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
				if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
					StartLocation = HitResult.Location;
				}
				getLocation = false;
			}
			else {
				DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
				if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
				{
					EndLocation = HitResult.Location;
				}
				GenerateRoad();
				isFirstClick = false;
			}
		}
		else {
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
			Cast<UStaticMeshComponent>(HitResult.GetComponent());
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
			{
				FVector ForwardVectorOfPrevRoad = (EndLocation - StartLocation).GetSafeNormal();
				FVector LeftVectorOfPrevRoad = FVector::CrossProduct(ForwardVectorOfPrevRoad, FVector::UpVector);
				FVector BackVectorOfPrevRoad = (StartLocation - EndLocation).GetSafeNormal();
				FVector RightVectorOfPrevRoad = FVector::CrossProduct(FVector::UpVector, ForwardVectorOfPrevRoad);

				StartLocation = EndLocation;
				EndLocation = HitResult.Location;

				float Distance = FVector::Dist(EndLocation, StartLocation);

				FVector ForwardVectorOfCurrRoad = EndLocation - StartLocation;
				float AngleOfPointWithLeftVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, LeftVectorOfPrevRoad);
				float AngleOfPointWithBackVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, BackVectorOfPrevRoad);

				FVector EndPointDir;
				if ((AngleOfPointWithLeftVecOfPrev >= 0 && AngleOfPointWithLeftVecOfPrev < 45) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev <= 90)) {
					EndPointDir = LeftVectorOfPrevRoad;
				}
				else if ((AngleOfPointWithLeftVecOfPrev > 135 && AngleOfPointWithLeftVecOfPrev <= 180) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev >= 90)) {
					EndPointDir = RightVectorOfPrevRoad;
				}
				else if (AngleOfPointWithLeftVecOfPrev > 45 && AngleOfPointWithLeftVecOfPrev <= 135) {
					EndPointDir = ForwardVectorOfPrevRoad;
				}
				EndLocation = StartLocation + (EndPointDir.GetSafeNormal() * Distance);
			}

			GenerateRoad();
		}
	}
	else {
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if (Cast<ARoadGenerator>(HitResult.GetActor()))
		{
			RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Visible);
			RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Visible);
			RoadGeneratorActor = Cast<ARoadGenerator>(HitResult.GetActor());
			RoadConstructionWidget->WidthValue->SetValue(RoadGeneratorActor->GetActorScale3D().Y * RoadDimensions.Y);
			RoadConstructionWidget->LocationX->SetValue(RoadGeneratorActor->GetActorLocation().X);
			RoadConstructionWidget->LocationY->SetValue(RoadGeneratorActor->GetActorLocation().Y);
		}
		else {
			RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
			RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void AArchVizController::GenerateRoad()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ActorSpawnLocation = (StartLocation + EndLocation) / 2;
	FRotator ActorSpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

	ARoadGenerator* SpawnedActor = GetWorld()->SpawnActor<ARoadGenerator>(ARoadGenerator::StaticClass(), ActorSpawnLocation, ActorSpawnRotation, SpawnParams);

	RoadDimensions.X = FVector::Dist(StartLocation, EndLocation);
	RoadDimensions.Y = 100;
	RoadDimensions.Z = 2;
	SpawnedActor->GenerateCube(FVector(RoadDimensions), FVector(0, 0, RoadDimensions.Z / 2), RoadMaterial);
}
float AArchVizController::FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2)
{
	FVector NormalizedVec1 = Vec1.GetSafeNormal();
	FVector NormalizedVec2 = Vec2.GetSafeNormal();
	float DotProduct = FVector::DotProduct(NormalizedVec1, NormalizedVec2);
	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
	float AngleRadians = FMath::Acos(DotProduct);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	return AngleDegrees;
}

// Road Widget Bind Function
void AArchVizController::GenerateNewRoad()
{
	isFirstClick = true;
	getLocation = true;
}
void AArchVizController::OnRoadWidthValueChanged(float InValue)
{
	if (RoadGeneratorActor) {
		RoadGeneratorActor->SetActorScale3D(FVector(1, InValue / RoadDimensions.Y, 1));
	}
}
void AArchVizController::OnRoadLocationXValueChanged(float InValue)
{
	if (RoadGeneratorActor) {
		RoadGeneratorActor->SetActorLocation(FVector(InValue, RoadGeneratorActor->GetActorLocation().Y, RoadGeneratorActor->GetActorLocation().Z));
	}
}
void AArchVizController::OnRoadLocationYValueChanged(float InValue)
{
	if (RoadGeneratorActor) {
		RoadGeneratorActor->SetActorLocation(FVector(RoadGeneratorActor->GetActorLocation().X, InValue, RoadGeneratorActor->GetActorLocation().Z));
	}
}
void AArchVizController::OnRoadModeToggleBtnClicked()
{
	if (CurrentRoadMode == ERodeMode::ConstructionMode) {
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Switch to Construction Mode"));
		CurrentRoadMode = ERodeMode::EditorMode;

		RoadConstructionWidget->RoadConstructionMsg->SetVisibility(ESlateVisibility::Visible);
		RoadConstructionWidget->RoadConstructionMsg->SetText(FText::FromString("Select the Road which you want to Edit."));
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Switch to Editor Mode"));
		CurrentRoadMode = ERodeMode::ConstructionMode;

		RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->RoadConstructionMsg->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Visible);
	}
}

// Building Editor Function
void AArchVizController::SetupBuildingEditorInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		BuildingEditorIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforSelectingBuildingComponent = NewObject<UInputAction>(this);
		ClickforSelectingBuildingComponent->ValueType = EInputActionValueType::Boolean;
		BuildingEditorIMC->MapKey(ClickforSelectingBuildingComponent, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforSelectingBuildingComponent, ETriggerEvent::Completed, this, &AArchVizController::SelectBuildingComponentOnClick);
	}
}
void AArchVizController::SelectBuildingComponentOnClick() {
	if(!bShouldEditWallLocationUnderCursor && !bShouldEditFloorLocationUnderCursor) {
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			WallGeneratorActor = Cast<AWallGenerator>(HitResult.GetActor());
			UStaticMeshComponent* SelectedStaticMeshComponent{};
			if (Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
				SelectedStaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
				float StaticMeshHeight = SelectedStaticMeshComponent->GetStaticMesh()->GetBounds().GetBox().GetSize().Z;

				if (StaticMeshHeight == WallGeneratorActor->HeightOfWall) {
					EditWall();
				}
				else {
					EditDoor();
				}
			}
			else if (Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
				EditWall();
			}
		}
		else if (Cast<AFloorGenerator>(HitResult.GetActor())) {
			FloorGeneratorActor = Cast<AFloorGenerator>(HitResult.GetActor());
			EditFloor();
		}
		else { // No Asset Selected
			// Wall Editor Widgets
			BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);

			// Door Editor Widgets
			BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);

			// Floor Editor Widgets
			BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else {
		if (WallGeneratorActor) {
			WallGeneratorActor = nullptr;
			bShouldEditWallLocationUnderCursor = false;

			BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (FloorGeneratorActor) {
			FloorGeneratorActor = nullptr;
			bShouldEditFloorLocationUnderCursor = false;

			BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void AArchVizController::EditWall() {
	// Wall Editor Widgets
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Visible);

	// Door Editor Widgets
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);

	// Floor Editor Widgets
	BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);

	if (WallGeneratorActor) {
		BuildingConstructionWidget->NoSegmentsValue->SetValue(WallGeneratorActor->WallStaticMeshComponentsArr.Num());
		BuildingConstructionWidget->LocationX->SetValue(WallGeneratorActor->GetActorLocation().X);
		BuildingConstructionWidget->LocationY->SetValue(WallGeneratorActor->GetActorLocation().Y);
	}
}
void AArchVizController::EditDoor() {
	// Door Editor Widgets
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Visible);

	// Wall Editor Widgets
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);

	// Floor Editor Widgets
	BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::EditFloor() {
	// Floor Editor Widgets
	BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Visible);
	if (FloorGeneratorActor) {
		BuildingConstructionWidget->FloorLocationX->SetValue(FloorGeneratorActor->GetActorLocation().X);
		BuildingConstructionWidget->FloorLocationY->SetValue(FloorGeneratorActor->GetActorLocation().Y);

		BuildingConstructionWidget->FloorDimensionX->SetValue(FloorGeneratorActor->FloorMeasurements.X);
		BuildingConstructionWidget->FloorDimensionY->SetValue(FloorGeneratorActor->FloorMeasurements.Y);
		BuildingConstructionWidget->FloorDimensionZ->SetValue(FloorGeneratorActor->FloorMeasurements.Z);
	}

	// Wall Editor Widgets
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);

	// Door Editor Widgets
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);
}

// Building Widget Bind Function
void AArchVizController::OnWallBtnClicked() {
	
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	//BuildingConstructionWidget->FloorDimensions->SetVisibility(ESlateVisibility::Hidden);

	CurrentBuildingComponent = EBuildingComponent::Wall;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

	if (WallGeneratorActorRef) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		WallGeneratorActor = GetWorld()->SpawnActor<AWallGenerator>(WallGeneratorActorRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		int32 Value;
		if (BuildingConstructionWidget) {
			Value = int32(BuildingConstructionWidget->NoSegmentsValue->GetValue());
		}
		if (WallGeneratorActor) {
			WallGeneratorActor->GenerateWall(Value);
		}
	}
}
void AArchVizController::OnDoorBtnClicked() {
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
	//BuildingConstructionWidget->FloorDimensions->SetVisibility(ESlateVisibility::Hidden);

	CurrentBuildingComponent = EBuildingComponent::Door;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

}
void AArchVizController::OnFloorBtnClicked() {
	//BuildingConstructionWidget->FloorDimensions->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);

	CurrentBuildingComponent = EBuildingComponent::Floor;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

}
void AArchVizController::OnRoofBtnClicked() {
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	//BuildingConstructionWidget->FloorDimensions->SetVisibility(ESlateVisibility::Hidden);

	CurrentBuildingComponent = EBuildingComponent::Roof;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

}
void AArchVizController::OnBuildingModeToggleBtnClicked()
{
	if (CurrentBuildingMode == EBuildingMode::ConstructionMode) {
		
		DestroyWallGeneratorActor();
		bIsInBuildingEditorMode = true;
		
		CurrentBuildingMode = EBuildingMode::EditorMode;
		BuildingConstructionWidget->BuildingModeToggleBtnText->SetText(FText::FromString("Switch to Construction Mode"));

		BuildingConstructionWidget->WallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DoorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->RoofBtn->SetVisibility(ESlateVisibility::Hidden);
		// Wall Widgets
		BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		// Door Widgets
		BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);
		// Floor Widgets
		BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		UpdateInputMappings();
	}
	else {
		bIsInBuildingEditorMode = false;
		bShouldEditWallLocationUnderCursor = false;
		bShouldEditFloorLocationUnderCursor = false;
		CurrentBuildingMode = EBuildingMode::ConstructionMode;

		if(WallGeneratorActor) {WallGeneratorActor = nullptr;}

		CurrentBuildingComponent = EBuildingComponent::None;
		BuildingConstructionWidget->BuildingModeToggleBtnText->SetText(FText::FromString("Switch to Editor Mode"));
		BuildingConstructionWidget->WallBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->DoorBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->FloorBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->RoofBtn->SetVisibility(ESlateVisibility::Visible);
		// Wall Widgets
		BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		// Door Widgets
		BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		// Floor Widgets
		BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		UpdateInputMappings();
	}
}

// Wall Construction
void AArchVizController::SetupWallConstructionInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		WallConstructionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforWallConstruction = NewObject<UInputAction>(this);
		ClickforWallConstruction->ValueType = EInputActionValueType::Boolean;
		WallConstructionIMC->MapKey(ClickforWallConstruction, EKeys::LeftMouseButton);

		UInputAction* RotationforWall = NewObject<UInputAction>(this);
		RotationforWall->ValueType = EInputActionValueType::Boolean;
		WallConstructionIMC->MapKey(RotationforWall, EKeys::R);

		EnhancedInputComponent->BindAction(ClickforWallConstruction, ETriggerEvent::Completed, this, &AArchVizController::BuildWallAtClick);
		EnhancedInputComponent->BindAction(RotationforWall, ETriggerEvent::Completed, this, &AArchVizController::RotateWall);
	}
}
void AArchVizController::BuildWallAtClick()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	WallGeneratorActor = GetWorld()->SpawnActor<AWallGenerator>(WallGeneratorActorRef, WallLocation, FRotator::ZeroRotator, SpawnParams);
	if(TempWallMesh){ WallGeneratorActor->WallStaticMesh = TempWallMesh; }
	WallGeneratorActor->GenerateWall(BuildingConstructionWidget->NoSegmentsValue->GetValue());
}
void AArchVizController::RotateWall()
{
	WallGeneratorActor->SetActorRelativeRotation(FRotator(0, WallGeneratorActor->GetActorRotation().Yaw + 90, 0));
}
void AArchVizController::DestroyWallGeneratorActor() {
	if (WallGeneratorActor) {
		if(!bIsInBuildingEditorMode) {WallGeneratorActor->Destroy();}
		WallGeneratorActor = nullptr;
	}
	bIsInBuildingEditorMode = false;
}
void AArchVizController::SnapActor(float SnapValue)
{
	if(WallGeneratorActor && CurrentBuildingComponent == EBuildingComponent::Wall) {
		auto CurrentLocation = WallGeneratorActor->GetActorLocation();
		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
		WallGeneratorActor->SetActorLocation(CurrentLocation);
	}
	else if (FloorGeneratorActor && CurrentBuildingComponent == EBuildingComponent::Floor) {
		auto CurrentLocation = FloorGeneratorActor->GetActorLocation();
		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
		FloorGeneratorActor->SetActorLocation(CurrentLocation);
	}
}

// Wall Bind Function
void AArchVizController::OnWallLocationXValueChanged(float InValue) {
	if (WallGeneratorActor) {
		FVector Location = WallGeneratorActor->GetActorLocation();
		WallGeneratorActor->SetActorLocation(FVector(InValue, Location.Y, Location.Z));
	}
}
void AArchVizController::OnWallLocationYValueChanged(float InValue) {
	if (WallGeneratorActor) {
		FVector Location = WallGeneratorActor->GetActorLocation();
		WallGeneratorActor->SetActorLocation(FVector(Location.X, InValue, Location.Z));
	}
}
void AArchVizController::OnSegmentsChanged(float InValue)
{
	if (WallGeneratorActor) {
		WallGeneratorActor->GenerateWall(InValue);
	}
}
void AArchVizController::SetWallStaticMesh(const FWallData& WallData) {
	if (IsValid(WallGeneratorActor)) {
		TempWallMesh = WallData.StaticMesh;
		WallGeneratorActor->WallStaticMesh = TempWallMesh;
		WallGeneratorActor->GenerateWall(BuildingConstructionWidget->NoSegmentsValue->GetValue());
	}
}
void AArchVizController::OnDestroyWallBtnClicked() {
	if (WallGeneratorActor) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
		bShouldEditWallLocationUnderCursor = false;

		BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AArchVizController::OnUpdateWallLocationUnderCursorBtnClicked() {
	if(WallGeneratorActor) {
		bShouldEditWallLocationUnderCursor = true;
	}
}

// Door Construction
void AArchVizController::SetupDoorConstructionInputs() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		DoorConstructionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforDoorConstruction = NewObject<UInputAction>(this);
		ClickforDoorConstruction->ValueType = EInputActionValueType::Boolean;
		DoorConstructionIMC->MapKey(ClickforDoorConstruction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforDoorConstruction, ETriggerEvent::Completed, this, &AArchVizController::GenerateDoorOnClick);
	}
}
void AArchVizController::GenerateDoorOnClick() {
	if(CurrentBuildingMode == EBuildingMode::ConstructionMode) {GetHitResultUnderCursor(ECC_Visibility, true, HitResult);}

	if (AWallGenerator* WallTempActor = Cast<AWallGenerator>(HitResult.GetActor())) {
		if (UStaticMeshComponent* DoorComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
			if (DoorMesh) {
				UProceduralMeshComponent* CubeComponent = NewObject<UProceduralMeshComponent>(WallTempActor);
				float DoorHeight = DoorMesh->GetBounds().GetBox().GetSize().Z;
				FVector WallDimensions = DoorComponent->GetStaticMesh()->GetBounds().GetBox().GetSize();

				if (WallDimensions.Z == WallTempActor->HeightOfWall) {
					FVector DoorLocation = DoorComponent->GetRelativeLocation();
					DoorLocation.Z -= (WallDimensions.Z / 2);

					FVector CubeDimensions = { WallDimensions.X, WallDimensions.Y, WallDimensions.Z - DoorHeight };
					FVector CubeLocation = { DoorLocation.X, DoorLocation.Y, DoorLocation.Z + DoorHeight + ((WallDimensions.Z - DoorHeight) / 2) };

					DoorComponent->SetRelativeRotation(FRotator(0, 90, 0));
					DoorComponent->SetRelativeLocation(DoorLocation);
					DoorComponent->SetStaticMesh(DoorMesh);

					CubeComponent->SetupAttachment(WallTempActor->GetRootComponent());
					CubeComponent->RegisterComponentWithWorld(GetWorld());

					WallTempActor->GenerateCube(CubeDimensions, FVector::ZeroVector, CubeComponent);

					CubeComponent->SetRelativeLocation(CubeLocation);
					CubeComponent->SetVisibility(true);

					int32 index = WallTempActor->WallStaticMeshComponentsArr.IndexOfByKey(Cast<UStaticMeshComponent>(HitResult.GetComponent()));
					WallTempActor->WallActorMap.Add(index, {DoorMesh, CubeComponent});
				}
				else {
					DoorComponent->SetRelativeRotation(FRotator(0, 90, 0));
					DoorComponent->SetStaticMesh(DoorMesh);
				}
			}
		}
	}
}

// Door Bind Function
void AArchVizController::SetDoorMesh(const FDoorData& DoorData) {
	DoorMesh = DoorData.StaticMesh;
	if (CurrentBuildingMode == EBuildingMode::EditorMode) {
		GenerateDoorOnClick();
	}
}
void AArchVizController::OnDestroyDoorBtnClicked() {
	if (WallGeneratorActor) {
		if (Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
			UStaticMeshComponent* Component = Cast<UStaticMeshComponent>(HitResult.GetComponent());
			int32 Key = WallGeneratorActor->WallStaticMeshComponentsArr.IndexOfByKey(Component);

			if (WallGeneratorActor->WallActorMap.Find(Key)->ProceduralMeshComponent) {
				WallGeneratorActor->WallActorMap.Find(Key)->ProceduralMeshComponent->DestroyComponent();
				WallGeneratorActor->WallActorMap.Find(Key)->ProceduralMeshComponent = nullptr;
			}

			WallGeneratorActor->WallActorMap.FindAndRemoveChecked(Key);
			FRotator FirstRotation = Component->GetRelativeRotation();
			Component->SetRelativeRotation(FRotator(0, 0, 0));
			Component->SetStaticMesh(WallGeneratorActor->WallStaticMesh);


			FVector Location = Component->GetRelativeLocation();
			Component->SetRelativeLocation(FVector(Location.X, Location.Y, Location.Z + (WallGeneratorActor->HeightOfWall / 2)));

			BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// Floor Construction
void AArchVizController::SetupFloorConstructionInputs() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		FloorConstructionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforStartingFloorConstruction = NewObject<UInputAction>(this);
		ClickforStartingFloorConstruction->ValueType = EInputActionValueType::Boolean;
		FloorConstructionIMC->MapKey(ClickforStartingFloorConstruction, EKeys::LeftMouseButton);

		UInputAction* ReleaseClickforCompletingFloorConstruction = NewObject<UInputAction>(this);
		ReleaseClickforCompletingFloorConstruction->ValueType = EInputActionValueType::Boolean;
		FloorConstructionIMC->MapKey(ReleaseClickforCompletingFloorConstruction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforStartingFloorConstruction, ETriggerEvent::Triggered, this, &AArchVizController::StartBuildingFloor);
		EnhancedInputComponent->BindAction(ReleaseClickforCompletingFloorConstruction, ETriggerEvent::Completed, this, &AArchVizController::CompleteBuildingFloor);
	}
}
void AArchVizController::StartBuildingFloor()
{
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!FloorGeneratorActor) {
		StartFloorLocation = HitResult.Location;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if(FloorGeneratorActorRef) {
			FloorGeneratorActor = GetWorld()->SpawnActor<AFloorGenerator>(FloorGeneratorActorRef, StartFloorLocation, FRotator::ZeroRotator, SpawnParams);
		}
	}
	else {
		FVector CurrentFloorLocation = HitResult.Location;

		if (FloorGeneratorActor) {
			FloorGeneratorActor->FloorMeasurements.X = FMath::Abs(CurrentFloorLocation.X - StartFloorLocation.X);
			FloorGeneratorActor->FloorMeasurements.Y = FMath::Abs(CurrentFloorLocation.Y - StartFloorLocation.Y);
			FloorGeneratorActor->FloorMeasurements.Z = 5.0f;

			/*float FloorLength = FMath::Abs(CurrentFloorLocation.X - StartFloorLocation.X);
			float FloorWidth = FMath::Abs(CurrentFloorLocation.Y - StartFloorLocation.Y);
			float FloorHeight = 5.0f;*/

			FloorGeneratorActor->SetActorLocation((StartFloorLocation + CurrentFloorLocation) / 2);
			FloorGeneratorActor->GenerateFloor(FloorGeneratorActor->FloorMeasurements);
			//FloorGeneratorActor->GenerateFloor({FloorLength, FloorWidth, FloorHeight});
			SnapActor(12.5);
		}
	}
}
void AArchVizController::CompleteBuildingFloor() {
	FloorGeneratorActor = nullptr;
}

// Floor Bind Function
void AArchVizController::OnFloorLocationXValueChanged(float InValue)
{
	if (FloorGeneratorActor) {
		FVector Location = FloorGeneratorActor->GetActorLocation();
		FloorGeneratorActor->SetActorLocation(FVector(InValue, Location.Y, Location.Z));
	}
}
void AArchVizController::OnFloorLocationYValueChanged(float InValue)
{
	if (FloorGeneratorActor) {
		FVector Location = FloorGeneratorActor->GetActorLocation();
		FloorGeneratorActor->SetActorLocation(FVector(Location.X, InValue, Location.Z));
	}
}
void AArchVizController::OnFloorDimensionXValueChanged(float InValue){
	if (FloorGeneratorActor) {
		FloorGeneratorActor->FloorMeasurements.X = InValue;
		FloorGeneratorActor->GenerateFloor(FloorGeneratorActor->FloorMeasurements);
	}
}
void AArchVizController::OnFloorDimensionYValueChanged(float InValue){
	if (FloorGeneratorActor) {
		FloorGeneratorActor->FloorMeasurements.Y = InValue;
		FloorGeneratorActor->GenerateFloor(FloorGeneratorActor->FloorMeasurements);
	}
}
void AArchVizController::OnFloorDimensionZValueChanged(float InValue){
	if (FloorGeneratorActor) {
		FloorGeneratorActor->FloorMeasurements.Z = InValue;
		FloorGeneratorActor->GenerateFloor(FloorGeneratorActor->FloorMeasurements);
	}
}
void AArchVizController::OnDestroyFloorBtnClicked(){
	if (FloorGeneratorActor) {
		FloorGeneratorActor->Destroy();
		FloorGeneratorActor = nullptr;
		bShouldEditFloorLocationUnderCursor = false;

		BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AArchVizController::OnUpdateFloorLocationUnderCursorBtnClicked(){
	if (FloorGeneratorActor) {
		bShouldEditFloorLocationUnderCursor = true;
	}
}