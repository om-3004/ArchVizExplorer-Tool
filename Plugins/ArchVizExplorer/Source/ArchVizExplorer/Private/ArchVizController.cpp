// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"

AArchVizController::AArchVizController() : getLocation{ true }, isFirstClick{ true }, bIsInRoadEditor{ false }, bIsInBuildingEditorMode{ false }, bShouldEditWallLocationUnderCursor{ false }, bShouldEditFloorLocationUnderCursor{ false }
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
void AArchVizController::PreviewDoor(){
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, DoorStaticMeshActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			AWallGenerator* TempWallActor = Cast<AWallGenerator>(HitResult.GetActor());
			if (DoorStaticMeshActor) {
				FVector WallDirection = TempWallActor->GetActorForwardVector(); // 1,0,0

				if(DoorMesh) {
					FVector PreviewDoorLocation;
					if ((FindAngleBetweenVectors({ 1, 0, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ -1, 0, 0 }, WallDirection) == 0)) { // X || -X
						if (HitResult.Location.Y > TempWallActor->GetActorLocation().Y) {
							PreviewDoorLocation = HitResult.Location + FVector{ 0, DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0 };
						}
						else {
							PreviewDoorLocation = HitResult.Location + FVector{ 0, -DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0 };
						}
					}
					else if ((FindAngleBetweenVectors({ 0, 1, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ 0, -1, 0 }, WallDirection) == 0)) {
						if (HitResult.Location.X > TempWallActor->GetActorLocation().X) {
							PreviewDoorLocation = HitResult.Location + FVector{ DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0, 0 };
						}
						else {
							PreviewDoorLocation = HitResult.Location + FVector{ -DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0, 0 };
						}
					}
					PreviewDoorLocation.Z = TempWallActor->GetActorLocation().Z;
					DoorStaticMeshActor->SetActorLocation(PreviewDoorLocation);
				}
			}
			else {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				DoorStaticMeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), HitResult.Location, TempWallActor->GetActorRotation() + FRotator(0, 90, 0), SpawnParams);
				DoorStaticMeshActor->SetMobility(EComponentMobility::Movable);
				DoorStaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(DoorMesh);
			}
		}
		else {
			DestroyDoorPreviewActor();
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
void AArchVizController::PreviewInteriorOnWall() {
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorDesignActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			AWallGenerator* TempWallActor = Cast<AWallGenerator>(HitResult.GetActor());
			if (IsValid(InteriorStaticMesh)) {
				if (InteriorDesignActor) {
					InteriorDesignActor->SetActorRotation(FRotator(0, 0, 0));
					FVector WallDirection = TempWallActor->GetActorForwardVector(); // 1,0,0

					FVector PreviewInteriorLocation{};
					if ((FindAngleBetweenVectors({ 1, 0, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ -1, 0, 0 }, WallDirection) == 0)) { // X || -X
						if (HitResult.Location.Y > TempWallActor->GetActorLocation().Y) {
							PreviewInteriorLocation = HitResult.Location + FVector{ 0, InteriorStaticMesh->GetBounds().GetBox().GetSize().Y / 2, 0 };
						}
						else {
							PreviewInteriorLocation = HitResult.Location + FVector{ 0, -InteriorStaticMesh->GetBounds().GetBox().GetSize().Y / 2, 0 };
							InteriorDesignActor->SetActorRotation(FRotator(0, 180, 0));
						}
					}
					else if ((FindAngleBetweenVectors({ 0, 1, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ 0, -1, 0 }, WallDirection) == 0)) {
						if (HitResult.Location.X > TempWallActor->GetActorLocation().X) {
							PreviewInteriorLocation = HitResult.Location + FVector{ InteriorStaticMesh->GetBounds().GetBox().GetSize().Y / 2, 0, 0 };
							InteriorDesignActor->SetActorRotation(FRotator(0, -90, 0));
						}
						else {
							PreviewInteriorLocation = HitResult.Location + FVector{ -InteriorStaticMesh->GetBounds().GetBox().GetSize().Y / 2, 0, 0 };
							InteriorDesignActor->SetActorRotation(FRotator(0, 90, 0));
						}
					}
					InteriorDesignActor->SetActorLocation(PreviewInteriorLocation);
				}
				else {
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), HitResult.Location, TempWallActor->GetActorRotation(), SpawnParams);
					InteriorDesignActor->SetInteriorStaticMesh(InteriorStaticMesh);

					InteriorDesignActor->InteriorStaticMeshComponent->SetRenderCustomDepth(true);
					InteriorDesignActor->InteriorStaticMeshComponent->CustomDepthStencilValue = 2.0;
				}
			}
		}
		else {
			DestroyInteriorPreviewActor();
		}
	}
}
void AArchVizController::PreviewInteriorOnFloor() {
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorDesignActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		if (AFloorGenerator* FloorActor = Cast<AFloorGenerator>(HitResult.GetActor())) {
			if(FVector(0, 0, (FloorActor->FloorMeasurements.Z + FloorActor->GetActorLocation().Z)).Equals(FVector(0, 0, HitResult.Location.Z)) && InteriorStaticMesh) {
					if (InteriorDesignActor) {
						InteriorDesignActor->SetActorLocation(HitResult.Location);
					}
					else {
						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), HitResult.Location, FRotator::ZeroRotator, SpawnParams);
						InteriorDesignActor->SetInteriorStaticMesh(InteriorStaticMesh);

						InteriorDesignActor->InteriorStaticMeshComponent->SetRenderCustomDepth(true);
						InteriorDesignActor->InteriorStaticMeshComponent->CustomDepthStencilValue = 2.0;
					}
			}
			else {
				DestroyInteriorPreviewActor();
			}
		}
		else {
			DestroyInteriorPreviewActor();
		}
	}
}
void AArchVizController::PreviewInteriorOnRoof(){
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorDesignActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		if (ARoofGenerator* RoofActor = Cast<ARoofGenerator>(HitResult.GetActor())) {
			{
				if (FVector(0, 0, RoofActor->GetActorLocation().Z).Equals(FVector(0, 0, HitResult.Location.Z)) && InteriorStaticMesh) {
					if (InteriorDesignActor) {
						InteriorDesignActor->SetActorLocation(HitResult.Location);
					}
					else {
						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), HitResult.Location, FRotator::ZeroRotator, SpawnParams);
						InteriorDesignActor->SetInteriorStaticMesh(InteriorStaticMesh);

						InteriorDesignActor->InteriorStaticMeshComponent->SetRenderCustomDepth(true);
						InteriorDesignActor->InteriorStaticMeshComponent->CustomDepthStencilValue = 2.0;
					}
				}
				else {
					DestroyInteriorPreviewActor();
				}
			}
		}
		else {
			DestroyInteriorPreviewActor();
		}
	}
}
void AArchVizController::PreviewTemplateActor(){
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, TemplateActor);
	TraceParams.AddIgnoredActors(IgnoreActorsForTemplateArray);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		if (TemplateActor) {
			TemplateActor->SetActorLocation(HitResult.Location);
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
		else if (CurrentBuildingMode == EBuildingMode::ConstructionMode && CurrentBuildingComponent == EBuildingComponent::Door) {
			PreviewDoor();
		}
		else if (CurrentBuildingComponent == EBuildingComponent::Template && TemplateActor) {
			PreviewTemplateActor();
		}
	}
	else if (CurrentSelectedMode == EModeSelected::InteriorDesign) {
		if(BuildingAssetSelectedController == EBuildingAsset::Wall){
			PreviewInteriorOnWall();
		}
		else if (BuildingAssetSelectedController == EBuildingAsset::Floor) {
			PreviewInteriorOnFloor();
		}
		else if (BuildingAssetSelectedController == EBuildingAsset::Roof) {
			PreviewInteriorOnRoof();
		}
	}
}

void AArchVizController::CreateWidgets() {
	if (StartMenuWidgetClassRef) {
		StartMenuWidget = CreateWidget<UStartMenuWidget>(this, StartMenuWidgetClassRef);
		StartMenuWidget->AddToViewport();
	}
	if (HomeWidgetClassRef) {
		HomeWidget = CreateWidget<UHomeWidget>(this, HomeWidgetClassRef);
		//HomeWidget->AddToViewport();
	}
	if (RoadConstructionWidgetClassRef) {
		RoadConstructionWidget = CreateWidget<URoadConstructionWidget>(this, RoadConstructionWidgetClassRef);
	}
	if (BuildingConstructionWidgetClassRef) {
		BuildingConstructionWidget = CreateWidget<UBuildingConstructionWidget>(this, BuildingConstructionWidgetClassRef);
	}
	if (MaterialSelectionWidgetClassRef) {
		MaterialSelectionWidget = CreateWidget<UMaterialSelectionWidget>(this, MaterialSelectionWidgetClassRef);
	}
	if (InteriorDesignWidgetClassRef) {
		InteriorDesignWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorDesignWidgetClassRef);
	}
	if (SaveLoadWidgetClassRef) {
		SaveLoadWidget = CreateWidget<USaveLoadWidget>(this, SaveLoadWidgetClassRef);
	}
}
void AArchVizController::BindWidgetDelegates() {
	if (StartMenuWidget && StartMenuWidgetClassRef) {
		StartMenuWidget->BlankProjectBtn->OnClicked.AddDynamic(this, &AArchVizController::CreateBlankProject);
		StartMenuWidget->ProjectListComboBox->OnSelectionChanged.AddDynamic(this, &AArchVizController::UpdateSelectedProject);
		StartMenuWidget->LoadProjectBtn->OnClicked.AddDynamic(this, &AArchVizController::ShowLoadProjectMenu);
		StartMenuWidget->CloseLoadProjectBtn->OnClicked.AddDynamic(this, &AArchVizController::HideLoadProjectMenu);
		StartMenuWidget->LoadBtn->OnClicked.AddDynamic(this, &AArchVizController::LoadSelectedProject);
	}
	if (HomeWidget && HomeWidgetClassRef) {
		HomeWidget->ModeSelectionDropdown->OnSelectionChanged.AddDynamic(this, &AArchVizController::OnModeSelectionChanged);
	}
	if (RoadConstructionWidget && RoadConstructionWidgetClassRef) {
		RoadConstructionWidget->ModeToggleBtn->OnClicked.AddDynamic(this, &AArchVizController::OnRoadModeToggleBtnClicked);
		RoadConstructionWidget->WidthValue->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadWidthValueChanged);
		RoadConstructionWidget->LocationX->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadLocationXValueChanged);
		RoadConstructionWidget->LocationY->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadLocationYValueChanged);
		RoadConstructionWidget->NewRoadBtn->OnClicked.AddDynamic(this, &AArchVizController::GenerateNewRoad);
		RoadConstructionWidget->DestroyRoadBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDestroyRoadBtnClicked);
	}
	if (BuildingConstructionWidget && BuildingConstructionWidgetClassRef) {
		BuildingConstructionWidget->WallBtn->OnClicked.AddDynamic(this, &AArchVizController::OnWallBtnClicked);
		BuildingConstructionWidget->DoorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDoorBtnClicked);
		BuildingConstructionWidget->FloorBtn->OnClicked.AddDynamic(this, &AArchVizController::OnFloorBtnClicked);
		BuildingConstructionWidget->RoofBtn->OnClicked.AddDynamic(this, &AArchVizController::OnRoofBtnClicked);
		BuildingConstructionWidget->BuildingModeToggleBtn->OnClicked.AddDynamic(this, &AArchVizController::OnBuildingModeToggleBtnClicked);
		BuildingConstructionWidget->KeyMappingBtn->OnClicked.AddDynamic(this, &AArchVizController::ToggleKeyMappingMenu);

		BuildingConstructionWidget->TemplateBtn->OnClicked.AddDynamic(this, &AArchVizController::OnTemplateBtnClicked);
		BuildingConstructionWidget->TemplateListComboBox->OnSelectionChanged.AddDynamic(this, &AArchVizController::UpdateSelectedTemplate);

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

		// Roof Generator
		BuildingConstructionWidget->RoofLocationX->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLocationXValueChanged);
		BuildingConstructionWidget->RoofLocationY->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLocationYValueChanged);
		BuildingConstructionWidget->RoofLocationZ->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLocationZValueChanged);
		BuildingConstructionWidget->RoofDimensionX->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofDimensionXValueChanged);
		BuildingConstructionWidget->RoofDimensionY->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofDimensionYValueChanged);
		BuildingConstructionWidget->RoofDimensionZ->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofDimensionZValueChanged);
		BuildingConstructionWidget->DestroyRoofBtn->OnClicked.AddDynamic(this, &AArchVizController::OnDestroyRoofBtnClicked);
	}
	if (MaterialSelectionWidget && MaterialSelectionWidgetClassRef) {
		MaterialSelectionWidget->RoadMaterialScrollBox->AfterRoadMaterialSelection.BindUFunction(this, "ApplyMaterialToRoad");
		MaterialSelectionWidget->BuildingMaterialScrollBox->AfterBuildingMaterialSelection.BindUFunction(this, "ApplyMaterialToBuildingComponents");
	}
	if (InteriorDesignWidget && InteriorDesignWidgetClassRef) {
		InteriorDesignWidget->WallInteriorScrollBox->AfterInteriorDesignSelection.BindUFunction(this, "SetInterior");
		InteriorDesignWidget->FloorInteriorScrollBox->AfterInteriorDesignSelection.BindUFunction(this, "SetInterior");
		InteriorDesignWidget->RoofInteriorScrollBox->AfterInteriorDesignSelection.BindUFunction(this, "SetInterior");
	}
	if (SaveLoadWidget && SaveLoadWidgetClassRef) {
		SaveLoadWidget->SaveTemplateBtn->OnClicked.AddDynamic(this, &AArchVizController::ShowSlotTypeMenu);
		SaveLoadWidget->LoadTemplateBtn->OnClicked.AddDynamic(this, &AArchVizController::ShowLoadMenu);
		SaveLoadWidget->CloseSaveMenuBtn->OnClicked.AddDynamic(this, &AArchVizController::HideSaveMenu);
		SaveLoadWidget->CloseLoadMenuBtn->OnClicked.AddDynamic(this, &AArchVizController::HideLoadMenu);
		SaveLoadWidget->SaveBtn->OnClicked.AddDynamic(this, &AArchVizController::SaveTemplate);
		SaveLoadWidget->ReplaceBtn->OnClicked.AddDynamic(this, &AArchVizController::ReplaceSlot);
		SaveLoadWidget->CancelBtn->OnClicked.AddDynamic(this, &AArchVizController::RewriteSlotName);
		SaveLoadWidget->CloseRenameMenuBtn->OnClicked.AddDynamic(this, &AArchVizController::HideRenameMenu);
		SaveLoadWidget->RenameBtn->OnClicked.AddDynamic(this, &AArchVizController::RenameSlot);

		SaveLoadWidget->NewSlotBtn->OnClicked.AddDynamic(this, &AArchVizController::ShowSaveMenu);
		SaveLoadWidget->CloseSlotTypeMenuBtn->OnClicked.AddDynamic(this, &AArchVizController::HideSlotTypeMenu);
		SaveLoadWidget->ExisitingSlotBtn->OnClicked.AddDynamic(this, &AArchVizController::ShowExistingSlotMenu);
		SaveLoadWidget->CloseSaveInExistingSlotMenuBtn->OnClicked.AddDynamic(this, &AArchVizController::CloseExistingSlotMenu);

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
	SetupRoofConstructionInputs(); 
	SetupMaterialMenuInputs();
	SetupInteriorDesignInputs();
	SetupTemplateInputs();
}

// Start Menu

void AArchVizController::CreateBlankProject(){
	if (StartMenuWidget && StartMenuWidget->IsInViewport()) {
		StartMenuWidget->RemoveFromParent();
	}
	if (HomeWidget) {
		HomeWidget->AddToViewport();
	}
}
void AArchVizController::LoadProjectList() {
	FString SavedDir = FPaths::ProjectSavedDir() + "SaveGames/";
	TArray<FString> ProjectNames = FindFiles(SavedDir, ".sav");

	StartMenuWidget->ProjectListComboBox->ClearOptions();
	StartMenuWidget->ProjectListComboBox->AddOption("-- Select a Project --");
	StartMenuWidget->ProjectListComboBox->SetSelectedOption("-- Select a Project --");

	for (int i{}; i < ProjectNames.Num(); ++i) {
		if (!ProjectNames[i].StartsWith(TEXT("Template"), 8, ESearchCase::CaseSensitive)) {
			StartMenuWidget->ProjectListComboBox->AddOption(ProjectNames[i].LeftChop(4));
		}
	}
}
void AArchVizController::ShowLoadProjectMenu() {
	StartMenuWidget->LoadProjectMenu->SetVisibility(ESlateVisibility::Visible);

	LoadProjectList();
}
void AArchVizController::HideLoadProjectMenu(){
	StartMenuWidget->LoadProjectMenu->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::UpdateSelectedProject(FString SelectedItem, ESelectInfo::Type SelectionType) {
	if(SelectedItem != "-- Select a Project --") { SelectedProjectName = SelectedItem; }
}
void AArchVizController::LoadSelectedProject(){
	if (!SelectedProjectName.IsEmpty()) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(BuildingTemplateIMC, 0);
		}
		if (StartMenuWidget && StartMenuWidget->IsInViewport()) {
			StartMenuWidget->RemoveFromParent();
		}
		//LoadProject();
		LoadSlotWithGivenName(FText::FromString(SelectedProjectName));

		if (StartMenuWidget && StartMenuWidget->IsInViewport()) {
			StartMenuWidget->RemoveFromParent();
		}
		if (HomeWidget) {
			HomeWidget->AddToViewport();
		}
	}
}

// Home Widget Bind Function
void AArchVizController::UpdateWidget() {
	switch (CurrentSelectedMode) {
	case EModeSelected::ViewMode:
		if (RoadConstructionWidget->IsInViewport() && RoadConstructionWidget) { RoadConstructionWidget->RemoveFromParent(); }
		if (BuildingConstructionWidget->IsInViewport() && BuildingConstructionWidget) { BuildingConstructionWidget->RemoveFromParent(); }
		if (MaterialSelectionWidget->IsInViewport() && MaterialSelectionWidget){ MaterialSelectionWidget->RemoveFromParent(); }
		if (InteriorDesignWidget->IsInViewport() && InteriorDesignWidget) { InteriorDesignWidget->RemoveFromParent(); }
		if (SaveLoadWidget->IsInViewport() && SaveLoadWidget) { SaveLoadWidget->RemoveFromParent(); }
		break;
	case EModeSelected::RoadConstruction:
		if (BuildingConstructionWidget->IsInViewport() && BuildingConstructionWidget) { BuildingConstructionWidget->RemoveFromParent(); }
		if (MaterialSelectionWidget->IsInViewport() && MaterialSelectionWidget){ MaterialSelectionWidget->RemoveFromParent(); }
		if (InteriorDesignWidget->IsInViewport() && InteriorDesignWidget) { InteriorDesignWidget->RemoveFromParent(); }
		if (SaveLoadWidget->IsInViewport() && SaveLoadWidget) { SaveLoadWidget->RemoveFromParent(); }
		if (RoadConstructionWidgetClassRef && RoadConstructionWidget) { RoadConstructionWidget->AddToViewport(); }
		break;
	case EModeSelected::BuildingConstruction:
		if (RoadConstructionWidget->IsInViewport() && RoadConstructionWidget) { RoadConstructionWidget->RemoveFromParent(); }
		if (MaterialSelectionWidget->IsInViewport() && MaterialSelectionWidget){ MaterialSelectionWidget->RemoveFromParent(); }
		if (InteriorDesignWidget->IsInViewport() && InteriorDesignWidget) { InteriorDesignWidget->RemoveFromParent(); }
		if (SaveLoadWidget->IsInViewport() && SaveLoadWidget) { SaveLoadWidget->RemoveFromParent(); }
		if (BuildingConstructionWidgetClassRef && BuildingConstructionWidget) { BuildingConstructionWidget->AddToViewport(); }
		break;
	case EModeSelected::MaterialSelection:
		if (RoadConstructionWidget->IsInViewport() && RoadConstructionWidget){ RoadConstructionWidget->RemoveFromParent(); }
		if (BuildingConstructionWidget->IsInViewport() && BuildingConstructionWidget) { BuildingConstructionWidget->RemoveFromParent(); }
		if (InteriorDesignWidget->IsInViewport() && InteriorDesignWidget) { InteriorDesignWidget->RemoveFromParent(); }
		if (SaveLoadWidget->IsInViewport() && SaveLoadWidget) { SaveLoadWidget->RemoveFromParent(); }
		if (MaterialSelectionWidgetClassRef && MaterialSelectionWidget){ MaterialSelectionWidget->AddToViewport(); }
		break;
	case EModeSelected::InteriorDesign:
		if (RoadConstructionWidget->IsInViewport() && RoadConstructionWidget) { RoadConstructionWidget->RemoveFromParent(); }
		if (BuildingConstructionWidget->IsInViewport() && BuildingConstructionWidget) { BuildingConstructionWidget->RemoveFromParent(); }
		if (MaterialSelectionWidget->IsInViewport() && MaterialSelectionWidget) { MaterialSelectionWidget->RemoveFromParent(); }
		if (SaveLoadWidget->IsInViewport() && SaveLoadWidget) { SaveLoadWidget->RemoveFromParent(); }
		if (InteriorDesignWidgetClassRef && InteriorDesignWidget){ InteriorDesignWidget->AddToViewport(); }
		break;
	case EModeSelected::SaveLoadMode:
		if (RoadConstructionWidget->IsInViewport() && RoadConstructionWidget) { RoadConstructionWidget->RemoveFromParent(); }
		if (BuildingConstructionWidget->IsInViewport() && BuildingConstructionWidget) { BuildingConstructionWidget->RemoveFromParent(); }
		if (MaterialSelectionWidget->IsInViewport() && MaterialSelectionWidget) { MaterialSelectionWidget->RemoveFromParent(); }
		if (InteriorDesignWidget->IsInViewport() && InteriorDesignWidget) { InteriorDesignWidget->RemoveFromParent(); }
		if (SaveLoadWidgetClassRef && SaveLoadWidget) { SaveLoadWidget->AddToViewport(); }
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
				Subsystem->AddMappingContext(RoofConstructionIMC, 0);
				break;
			case EBuildingComponent::Template:
				Subsystem->AddMappingContext(BuildingTemplateIMC, 0);
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
		case EModeSelected::MaterialSelection:
			Subsystem->AddMappingContext(MaterialSelectionIMC, 0);
			break;
		case EModeSelected::InteriorDesign:
			Subsystem->AddMappingContext(InteriorDesignIMC, 0);
			break;
		case EModeSelected::SaveLoadMode:
			break;
		}
	}
}
void AArchVizController::SetDefaultMode() {
	switch (CurrentSelectedMode) {
	case EModeSelected::RoadConstruction:
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Switch to Editor Mode"));
		CurrentRoadMode = ERodeMode::ConstructionMode;
		bIsInRoadEditor = false;
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Visible);
		RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->DestroyRoadBtn->SetVisibility(ESlateVisibility::Hidden);

		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button at desired location to generate Road."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
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
		BuildingConstructionWidget->TemplateBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->KeyMappingBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->KeyMappingBtntxt->SetText(FText::FromString("Show Key Mapping"));
		BuildingConstructionWidget->KeyMappingMenu->SetVisibility(ESlateVisibility::Hidden);

		BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);


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

		// Roof Editor Widgets
		BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);

		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Select the Building Component you want to generate by Generator Button."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
		break;
	case EModeSelected::MaterialSelection:
		MaterialSelectionWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialSelectionWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Select the Asset and choose Material to apply."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
		break;
	case EModeSelected::InteriorDesign:
		InteriorDesignWidget->WallInteriorScrollBox->SetVisibility(ESlateVisibility::Visible);
		InteriorDesignWidget->FloorInteriorScrollBox->SetVisibility(ESlateVisibility::Visible);
		InteriorDesignWidget->RoofInteriorScrollBox->SetVisibility(ESlateVisibility::Visible);

		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Choose the Interior and press Left Mouse Button to place it."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
		break;
	case EModeSelected::SaveLoadMode:
		SaveLoadWidget->SaveTemplateBtn->SetVisibility(ESlateVisibility::Visible);
		SaveLoadWidget->LoadTemplateBtn->SetVisibility(ESlateVisibility::Visible);
		SaveLoadWidget->SaveMenu->SetVisibility(ESlateVisibility::Hidden);
		SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Hidden);
		SaveLoadWidget->ConfirmationBox->SetVisibility(ESlateVisibility::Hidden);
		SaveLoadWidget->SlotNameTxt->SetText(FText::FromString(""));
	}
}
void AArchVizController::OnModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == FString("View Mode")) {
		CurrentSelectedMode = EModeSelected::ViewMode;
		RemovePostProcessMaterial();

		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		DestroyFloorPreviewActor();
		DestroyInteriorPreviewActor();

		UpdateWidget();
		UpdateInputMappings();
	}
	else if (SelectedItem == FString("Road Construction")) {
		if(RoadGeneratorActor){ RoadGeneratorActor = nullptr; }
		CurrentSelectedMode = EModeSelected::RoadConstruction;
		RemovePostProcessMaterial();
		SetDefaultMode();

		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		DestroyFloorPreviewActor();
		DestroyInteriorPreviewActor();

		if (RoadConstructionWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
	}
	else if (SelectedItem == FString("Building Construction")) {
		CurrentSelectedMode = EModeSelected::BuildingConstruction;
		RemovePostProcessMaterial();
		if(WallGeneratorActor){ WallGeneratorActor = nullptr; }
		if(FloorGeneratorActor){ FloorGeneratorActor = nullptr; }
		if(RoofGeneratorActor){ RoofGeneratorActor = nullptr; }
		SetDefaultMode();

		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		DestroyFloorPreviewActor();
		DestroyInteriorPreviewActor();

		if (BuildingConstructionWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
	}
	else if (SelectedItem == FString("Material Selection")) {
		CurrentSelectedMode = EModeSelected::MaterialSelection;
		SetDefaultMode();
		RemovePostProcessMaterial();

		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		DestroyFloorPreviewActor();
		DestroyInteriorPreviewActor();

		if (MaterialSelectionWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
	}
	else if (SelectedItem == FString("Interior Design")) {
		CurrentSelectedMode = EModeSelected::InteriorDesign;
		SetDefaultMode();
		RemovePostProcessMaterial();

		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		DestroyFloorPreviewActor();
		DestroyInteriorPreviewActor();

		if (InteriorDesignWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	else if (SelectedItem == FString("Save/Load Mode")) {
		CurrentSelectedMode = EModeSelected::SaveLoadMode;
		RemovePostProcessMaterial();

		SetDefaultMode();

		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		DestroyFloorPreviewActor();
		DestroyInteriorPreviewActor();

		if (SaveLoadWidgetClassRef) {
			UpdateWidget();
			UpdateInputMappings();
		}
	}
}
void AArchVizController::ToggleKeyMappingMenu() {
	if (BuildingConstructionWidget->KeyMappingBtntxt->GetText().EqualTo(FText::FromString("Show Key Mapping"))) {
		BuildingConstructionWidget->KeyMappingBtntxt->SetText(FText::FromString("Hide Key Mapping"));
		BuildingConstructionWidget->KeyMappingMenu->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		BuildingConstructionWidget->KeyMappingBtntxt->SetText(FText::FromString("Show Key Mapping"));
		BuildingConstructionWidget->KeyMappingMenu->SetVisibility(ESlateVisibility::Hidden);
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
		
		UInputAction* DestroyRoadAction = NewObject<UInputAction>(this);
		DestroyRoadAction->ValueType = EInputActionValueType::Boolean;
		RoadConstructionIMC->MapKey(DestroyRoadAction, EKeys::Delete);

		EnhancedInputComponent->BindAction(ClickforRoadConstruction, ETriggerEvent::Completed, this, &AArchVizController::GetRoadLocationOnClick);
		EnhancedInputComponent->BindAction(DestroyRoadAction, ETriggerEvent::Completed, this, &AArchVizController::OnDestroyRoadBtnClicked);
	}
}
void AArchVizController::GetRoadLocationOnClick()
{
	if (CurrentRoadMode == ERodeMode::ConstructionMode) {
		bIsInRoadEditor = false;
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
					StartLocation += (ForwardVectorOfPrevRoad * 125) + (RightVectorOfPrevRoad * 125);
					EndPointDir = LeftVectorOfPrevRoad;
				}
				else if ((AngleOfPointWithLeftVecOfPrev > 135 && AngleOfPointWithLeftVecOfPrev <= 180) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev >= 90)) {
					StartLocation += (ForwardVectorOfPrevRoad * 125) + (LeftVectorOfPrevRoad * 125);
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
		bIsInRoadEditor = true;
		if (RoadGeneratorActor) { RoadGeneratorActor->RoadProceduralMeshComponent->SetRenderCustomDepth(false); }
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if (Cast<ARoadGenerator>(HitResult.GetActor()))
		{
			RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Visible);
			RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Visible);
			RoadConstructionWidget->DestroyRoadBtn->SetVisibility(ESlateVisibility::Visible);
			RoadGeneratorActor = Cast<ARoadGenerator>(HitResult.GetActor());
			RoadConstructionWidget->WidthValue->SetValue(RoadGeneratorActor->GetActorScale3D().Y * RoadDimensions.Y);
			RoadConstructionWidget->LocationX->SetValue(RoadGeneratorActor->GetActorLocation().X);
			RoadConstructionWidget->LocationY->SetValue(RoadGeneratorActor->GetActorLocation().Y);

			RoadGeneratorActor->RoadProceduralMeshComponent->SetRenderCustomDepth(true);
			RoadGeneratorActor->RoadProceduralMeshComponent->CustomDepthStencilValue = 2.0;
		}
		else {
			RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
			RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
			RoadConstructionWidget->DestroyRoadBtn->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void AArchVizController::GenerateRoad()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ActorSpawnLocation = (StartLocation + EndLocation) / 2;
	FRotator ActorSpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

	RoadGeneratorActor = GetWorld()->SpawnActor<ARoadGenerator>(RoadGeneratorActorRef, ActorSpawnLocation, ActorSpawnRotation, SpawnParams);

	RoadDimensions.X = FVector::Dist(StartLocation, EndLocation);
	RoadDimensions.Y = 250;
	RoadDimensions.Z = 2;
	RoadGeneratorActor->GenerateRoad(RoadDimensions);
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

		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Select the Road which you want to Edit."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Switch to Editor Mode"));
		CurrentRoadMode = ERodeMode::ConstructionMode;
		if (RoadGeneratorActor) { RoadGeneratorActor->RoadProceduralMeshComponent->SetRenderCustomDepth(false); }

		RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->DestroyRoadBtn->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Visible);
	}
}
void AArchVizController::OnDestroyRoadBtnClicked(){
	if (RoadGeneratorActor && bIsInRoadEditor) {
		RoadGeneratorActor->Destroy();
		RoadGeneratorActor = nullptr;

		isFirstClick = true;
		getLocation = true;

		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Selected Road destroyed. you have to press LMB 2 times to generate another road."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
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
		
		UInputAction* RotateSelectedWallActor = NewObject<UInputAction>(this);
		RotateSelectedWallActor->ValueType = EInputActionValueType::Boolean;
		BuildingEditorIMC->MapKey(RotateSelectedWallActor, EKeys::R);

		EnhancedInputComponent->BindAction(ClickforSelectingBuildingComponent, ETriggerEvent::Completed, this, &AArchVizController::SelectBuildingComponentOnClick);
		EnhancedInputComponent->BindAction(RotateSelectedWallActor, ETriggerEvent::Completed, this, &AArchVizController::RotateWall);
	}
}
void AArchVizController::SelectBuildingComponentOnClick() {
	RemovePostProcessMaterial();

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
					if (Cast<UStaticMeshComponent>(HitResult.GetComponent()))
					{
						DoorHighlightComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
						DoorHighlightComponent->SetRenderCustomDepth(true);
						DoorHighlightComponent->CustomDepthStencilValue = 2.0;
					}
					EditDoor();
				}
			}
			else if (Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
				EditWall();
			}
		}
		else if (Cast<AFloorGenerator>(HitResult.GetActor())) {
			FloorGeneratorActor = Cast<AFloorGenerator>(HitResult.GetActor());
			FloorGeneratorActor->ApplyMaterialToFloorActor(FloorGeneratorActor->FloorMaterial);
			FloorGeneratorActor->FloorProceduralMeshComponent->SetRenderCustomDepth(true);
			FloorGeneratorActor->FloorProceduralMeshComponent->CustomDepthStencilValue = 2.0;

			EditFloor();
		}
		else if (Cast<ARoofGenerator>(HitResult.GetActor())) {
			RoofGeneratorActor = Cast<ARoofGenerator>(HitResult.GetActor());
			RoofGeneratorActor->ApplyMaterialToRoofActor(RoofGeneratorActor->RoofMaterial);
			RoofGeneratorActor->RoofProceduralMeshComponent->SetRenderCustomDepth(true);
			RoofGeneratorActor->RoofProceduralMeshComponent->CustomDepthStencilValue = 2.0;
			EditRoof();
		}
		else { // No Asset Selected
			RoadGeneratorActor = nullptr;
			WallGeneratorActor = nullptr;
			FloorGeneratorActor = nullptr;
			RoofGeneratorActor = nullptr;

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
	if (WallGeneratorActor) {
		BuildingConstructionWidget->NoSegmentsValue->SetValue(WallGeneratorActor->WallStaticMeshComponentsArr.Num());
		BuildingConstructionWidget->LocationX->SetValue(WallGeneratorActor->GetActorLocation().X);
		BuildingConstructionWidget->LocationY->SetValue(WallGeneratorActor->GetActorLocation().Y);

		for (int i{}; i < WallGeneratorActor->WallStaticMeshComponentsArr.Num(); i++) {
			if (WallGeneratorActor->HeightOfWall == WallGeneratorActor->WallStaticMeshComponentsArr[i]->GetStaticMesh()->GetBounds().GetBox().GetSize().Z)
			{
				WallGeneratorActor->WallStaticMeshComponentsArr[i]->SetRenderCustomDepth(true);
				WallGeneratorActor->WallStaticMeshComponentsArr[i]->CustomDepthStencilValue = 2.0;
			}
		}
		for (auto& WallProcedural : WallGeneratorActor->WallActorMap) {
			WallProcedural.Value.ProceduralMeshComponent->SetRenderCustomDepth(true);
			WallProcedural.Value.ProceduralMeshComponent->CustomDepthStencilValue = 2.0;
		}
		WallGeneratorActor->ApplyMaterialToWallActor(WallGeneratorActor->WallMaterial);
	}

	// Door Editor Widgets
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);

	// Floor Editor Widgets
	BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);

	// Roof Editor Widgets
	BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);
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

	// Roof Editor Widgets
	BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);
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

	// Roof Editor Widgets
	BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::EditRoof() {
	// Roof Editor Widgets
	BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Visible);
	if (RoofGeneratorActor) {
		BuildingConstructionWidget->RoofLocationX->SetValue(RoofGeneratorActor->GetActorLocation().X);
		BuildingConstructionWidget->RoofLocationY->SetValue(RoofGeneratorActor->GetActorLocation().Y);
		BuildingConstructionWidget->RoofLocationZ->SetValue(RoofGeneratorActor->GetActorLocation().Z);

		BuildingConstructionWidget->RoofDimensionX->SetValue(RoofGeneratorActor->RoofMeasurements.X);
		BuildingConstructionWidget->RoofDimensionY->SetValue(RoofGeneratorActor->RoofMeasurements.Y);
		BuildingConstructionWidget->RoofDimensionZ->SetValue(RoofGeneratorActor->RoofMeasurements.Z);
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

	// Floor Editor Widgets
	BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::RemovePostProcessMaterial() {
	if (RoadGeneratorActor && RoadGeneratorActor->RoadProceduralMeshComponent) {
		RoadGeneratorActor->RoadProceduralMeshComponent->SetRenderCustomDepth(false);
	}
	if (WallGeneratorActor) {
		for (int i{}; i < WallGeneratorActor->WallStaticMeshComponentsArr.Num(); i++) {
			if(WallGeneratorActor->WallStaticMeshComponentsArr[i]) { WallGeneratorActor->WallStaticMeshComponentsArr[i]->SetRenderCustomDepth(false); }
		}
		for (auto& WallProcedural : WallGeneratorActor->WallActorMap) {
			if(WallProcedural.Value.ProceduralMeshComponent) {WallProcedural.Value.ProceduralMeshComponent->SetRenderCustomDepth(false);}
		}
	}
	if (DoorHighlightComponent) {
		DoorHighlightComponent->SetRenderCustomDepth(false);
	}
	if (FloorGeneratorActor && FloorGeneratorActor->FloorProceduralMeshComponent) {
		FloorGeneratorActor->FloorProceduralMeshComponent->SetRenderCustomDepth(false);
	}
	if (RoofGeneratorActor && RoofGeneratorActor->RoofProceduralMeshComponent) {
		RoofGeneratorActor->RoofProceduralMeshComponent->SetRenderCustomDepth(false);
	}
	if (InteriorDesignActor && InteriorDesignActor->InteriorStaticMeshComponent) {
		InteriorDesignActor->InteriorStaticMeshComponent->SetRenderCustomDepth(false);
	}
}

// Building Widget Bind Function
void AArchVizController::OnWallBtnClicked() {
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button to place Wall at your desired location."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);

	DestroyDoorPreviewActor();
	DestroyFloorPreviewActor();

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
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button to place Door at your desired Wall location."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

	DestroyDoorPreviewActor();

	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);

	CurrentBuildingComponent = EBuildingComponent::Door;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

}
void AArchVizController::OnFloorBtnClicked() {
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press and Hold Left Mouse Button to generate Floor at your desired location."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

	DestroyDoorPreviewActor();

	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);

	CurrentBuildingComponent = EBuildingComponent::Floor;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

}
void AArchVizController::OnRoofBtnClicked() {
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press and Hold LMB to generate Roof at your desired location or press MMB between 4 walls."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

	DestroyDoorPreviewActor();

	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);

	CurrentBuildingComponent = EBuildingComponent::Roof;
	DestroyWallGeneratorActor();
	UpdateInputMappings();

}
void AArchVizController::OnTemplateBtnClicked() {
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Select a Building Template and place at your desired location using LMB."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

	DestroyWallGeneratorActor();
	DestroyDoorPreviewActor();
	DestroyFloorPreviewActor();
	
	BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Visible);

	LoadTemplateList();

	CurrentBuildingComponent = EBuildingComponent::Template;
	UpdateInputMappings();

}
void AArchVizController::OnBuildingModeToggleBtnClicked()
{
	if (CurrentBuildingMode == EBuildingMode::ConstructionMode) {
		
		DestroyWallGeneratorActor();
		DestroyDoorPreviewActor();
		bIsInBuildingEditorMode = true;
		
		CurrentBuildingMode = EBuildingMode::EditorMode;
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button to select the Building Component you want to edit."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

		BuildingConstructionWidget->BuildingModeToggleBtnText->SetText(FText::FromString("Switch to Construction Mode"));

		BuildingConstructionWidget->WallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DoorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->RoofBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->TemplateBtn->SetVisibility(ESlateVisibility::Hidden);
		// Wall Editor Widgets
		BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
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
		// Roof Editor Widgets
		BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);

		UpdateInputMappings();
	}
	else {
		RemovePostProcessMaterial();
		bIsInBuildingEditorMode = false;
		if(bShouldEditWallLocationUnderCursor){ DestroyWallGeneratorActor(); }
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
		BuildingConstructionWidget->TemplateBtn->SetVisibility(ESlateVisibility::Visible);
		BuildingConstructionWidget->TemplateListComboBox->SetVisibility(ESlateVisibility::Hidden);
		// Wall Editor Widgets
		BuildingConstructionWidget->SegmentBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->WallScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyWallBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateWallLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		// Door Editor Widgets
		BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		// Floor Editor Widgets
		BuildingConstructionWidget->FloorLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->FloorDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyFloorBtn->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->UpdateFloorLocationUnderCursorBtn->SetVisibility(ESlateVisibility::Hidden);
		// Roof Editor Widgets
		BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);

		UpdateInputMappings();
	}
}

// Template
void AArchVizController::SetupTemplateInputs() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		BuildingTemplateIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickToPlaceTemplateActor = NewObject<UInputAction>(this);
		ClickToPlaceTemplateActor->ValueType = EInputActionValueType::Boolean;
		BuildingTemplateIMC->MapKey(ClickToPlaceTemplateActor, EKeys::LeftMouseButton);

		UInputAction* RotateAction = NewObject<UInputAction>(this);
		RotateAction->ValueType = EInputActionValueType::Boolean;
		BuildingTemplateIMC->MapKey(RotateAction, EKeys::R);

		EnhancedInputComponent->BindAction(ClickToPlaceTemplateActor, ETriggerEvent::Completed, this, &AArchVizController::PlaceTemplateOnClick);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &AArchVizController::RotateTemplate);
	}
}
void AArchVizController::PlaceTemplateOnClick() {
	if (TemplateActor) {
		TemplateActor = nullptr;
		IgnoreActorsForTemplateArray.Empty();

		CurrentSelectedMode = EModeSelected::BuildingConstruction;
		HomeWidget->ModeSelectionDropdown->SetSelectedOption(FString("Building Construction"));
		OnModeSelectionChanged(FString("Building Construction"), ESelectInfo::Direct);
		if (HomeWidget && HomeWidgetClassRef) {
			HomeWidget->AddToViewport();
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(BuildingTemplateIMC);
		}
	}
}
void AArchVizController::RotateTemplate() {
	if (TemplateActor) {
		TemplateActor->SetActorRotation(FRotator(0, TemplateActor->GetActorRotation().Yaw + 90, 0));
	}
}

void AArchVizController::LoadTemplateList() {
	FString SavedTemplateDir = FPaths::ProjectSavedDir() + "Templates/";
	TArray<FString> TemplateNames = FindFiles(SavedTemplateDir, ".sav");

	BuildingConstructionWidget->TemplateListComboBox->ClearOptions();
	BuildingConstructionWidget->TemplateListComboBox->AddOption("-- Select a Template --");
	BuildingConstructionWidget->TemplateListComboBox->SetSelectedOption("-- Select a Template --");

	for (int i{}; i < TemplateNames.Num(); ++i) {
		BuildingConstructionWidget->TemplateListComboBox->AddOption(TemplateNames[i].LeftChop(4));
	}
}
void AArchVizController::UpdateSelectedTemplate(FString SelectedItem, ESelectInfo::Type SelectionType) {
	if (SelectedItem != "-- Select a Template --") {
		SelectedTemplateName = SelectedItem;

		if (!SelectedTemplateName.IsEmpty()) {
			LoadTemplate();
		}
	}
}
void AArchVizController::LoadTemplate() {
	UArchVizExplorerSaveGame* LoadGameInstance = Cast<UArchVizExplorerSaveGame>(UGameplayStatics::LoadGameFromSlot(SelectedTemplateName, 0));

	if (LoadGameInstance)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		TemplateActor = GetWorld()->SpawnActor<ATemplateActor>(ATemplateActor::StaticClass(), LoadGameInstance->WallActorArray[0].WallTransform.GetLocation(), FRotator::ZeroRotator, SpawnParams);

		TArray<AActor*> Actors;
		for (const FRoadSaveData& RoadData : LoadGameInstance->RoadActorArray)
		{
			ARoadGenerator* RoadActor = GetWorld()->SpawnActor<ARoadGenerator>(RoadGeneratorActorRef, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			RoadActor->SetActorTransform(RoadData.RoadTransform);

			RoadActor->ApplyMaterialToRoadActor(RoadData.RoadMaterial);
			RoadActor->GenerateRoad(RoadData.RoadDimensions);

			IgnoreActorsForTemplateArray.Add(RoadActor);
			RoadActor->AttachToActor(TemplateActor, FAttachmentTransformRules::KeepWorldTransform);

		}
		for (const FWallSaveData& WallData : LoadGameInstance->WallActorArray)
		{
			AWallGenerator* WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallGeneratorActorRef, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			WallActor->SetActorTransform(WallData.WallTransform);

			WallActor->WallStaticMesh = WallData.WallStaticMesh;
			WallActor->WallActorMap = WallData.WallActorMap;
			WallActor->GenerateWall(WallData.NoOfSegments);
			WallActor->ApplyMaterialToWallActor(WallData.WallMaterial);

			IgnoreActorsForTemplateArray.Add(WallActor);
			WallActor->AttachToActor(TemplateActor, FAttachmentTransformRules::KeepWorldTransform);
		}
		for (const FFloorSaveData& FloorData : LoadGameInstance->FloorActorArray)
		{
			AFloorGenerator* FloorActor = GetWorld()->SpawnActor<AFloorGenerator>(FloorGeneratorActorRef, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			FloorActor->SetActorTransform(FloorData.FloorTransform);
			FloorActor->GenerateFloor(FloorData.FloorDimensions);
			FloorActor->ApplyMaterialToFloorActor(FloorData.FloorMaterial);

			IgnoreActorsForTemplateArray.Add(FloorActor);
			FloorActor->AttachToActor(TemplateActor, FAttachmentTransformRules::KeepWorldTransform);
		}
		for (const FRoofSaveData& RoofData : LoadGameInstance->RoofActorArray)
		{
			ARoofGenerator* RoofActor = GetWorld()->SpawnActor<ARoofGenerator>(RoofGeneratorActorRef, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			RoofActor->SetActorTransform(RoofData.RoofTransform);

			RoofActor->GenerateRoof(RoofData.RoofDimensions);
			RoofActor->ApplyMaterialToRoofActor(RoofData.RoofMaterial);

			IgnoreActorsForTemplateArray.Add(RoofActor);
			RoofActor->AttachToActor(TemplateActor, FAttachmentTransformRules::KeepWorldTransform);
		}
		for (const FInteriorSaveData& InteriorData : LoadGameInstance->InteriorActorArray) {
			AInteriorDesign* InteriorActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			InteriorActor->SetActorTransform(InteriorData.InteriorTransform);

			InteriorActor->BuildingAssetForInterior = InteriorData.BuildingAssetForInterior;
			InteriorActor->InteriorStaticMesh = InteriorData.InteriorMesh;
			InteriorActor->SetInteriorStaticMesh(InteriorActor->InteriorStaticMesh);

			IgnoreActorsForTemplateArray.Add(InteriorActor);
			InteriorActor->AttachToActor(TemplateActor, FAttachmentTransformRules::KeepWorldTransform);
		}
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
	if(IsValid(WallGeneratorActor)) {WallGeneratorActor->SetActorRelativeRotation(FRotator(0, WallGeneratorActor->GetActorRotation().Yaw + 90, 0));}
}
void AArchVizController::DestroyWallGeneratorActor() {
	if (WallGeneratorActor) {
		if(!bIsInBuildingEditorMode) {WallGeneratorActor->Destroy();}
		if(bShouldEditWallLocationUnderCursor){WallGeneratorActor->Destroy(); bShouldEditWallLocationUnderCursor = false;}
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

		for (int i{}; i < WallGeneratorActor->WallStaticMeshComponentsArr.Num(); i++) {
			if (WallGeneratorActor->HeightOfWall == WallGeneratorActor->WallStaticMeshComponentsArr[i]->GetStaticMesh()->GetBounds().GetBox().GetSize().Z)
			{
				WallGeneratorActor->WallStaticMeshComponentsArr[i]->SetRenderCustomDepth(true);
				WallGeneratorActor->WallStaticMeshComponentsArr[i]->CustomDepthStencilValue = 2.0;
			}
		}
		for (auto& WallProcedural : WallGeneratorActor->WallActorMap) {
			WallProcedural.Value.ProceduralMeshComponent->SetRenderCustomDepth(true);
			WallProcedural.Value.ProceduralMeshComponent->CustomDepthStencilValue = 2.0;
		}

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
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button to Place the Wall at desired location."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
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
	DestroyDoorPreviewActor();

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
					WallTempActor->Indices.Add(index);
				}
				else {
					DoorComponent->SetRelativeRotation(FRotator(0, 90, 0));
					DoorComponent->SetStaticMesh(DoorMesh);
					//DoorComponent->SetMaterial(0, NONE);
				}
				DoorComponent->SetMaterial(0, nullptr);
			}
		}
		else if (Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
			UProceduralMeshComponent* ProceduralComponent = Cast<UProceduralMeshComponent>(HitResult.GetComponent());
			int key{};
			for (int i = 0; i < WallTempActor->Indices.Num(); i++) {
				if (WallTempActor->WallActorMap[WallTempActor->Indices[i]].ProceduralMeshComponent == ProceduralComponent)
				{
					key = WallTempActor->Indices[i];
					break;
				}
			}
			if (WallTempActor->WallStaticMeshComponentsArr[key]) {
				WallTempActor->WallStaticMeshComponentsArr[key]->SetRelativeRotation(FRotator(0, 90, 0));
				WallTempActor->WallStaticMeshComponentsArr[key]->SetStaticMesh(DoorMesh);
			}
		}
	}
	else {
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Door can only be placed on a Wall."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	}
}
void AArchVizController::DestroyDoorPreviewActor() {
	if (DoorStaticMeshActor) {
		DoorStaticMeshActor->Destroy();
		DoorStaticMeshActor = nullptr;
	}
}

// Door Bind Function
void AArchVizController::SetDoorMesh(const FDoorData& DoorData) {
	if(DoorData.StaticMesh) {DoorMesh = DoorData.StaticMesh;}
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
			Component->SetMaterial(0, WallGeneratorActor->WallMaterial);


			FVector Location = Component->GetRelativeLocation();
			Component->SetRelativeLocation(FVector(Location.X, Location.Y, Location.Z + (WallGeneratorActor->HeightOfWall / 2)));

			BuildingConstructionWidget->DoorScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
			BuildingConstructionWidget->DestroyDoorBtn->SetVisibility(ESlateVisibility::Hidden);
			RemovePostProcessMaterial();
		}
	}
	WallGeneratorActor = nullptr;
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
			float FloorLength = FMath::Abs(CurrentFloorLocation.X - StartFloorLocation.X);
			float FloorWidth = FMath::Abs(CurrentFloorLocation.Y - StartFloorLocation.Y);
			float FloorHeight = 5.0f;

			FloorGeneratorActor->SetActorLocation((StartFloorLocation + CurrentFloorLocation) / 2);
			FloorGeneratorActor->GenerateFloor({FloorLength, FloorWidth, FloorHeight});
			SnapActor(12.5);
		}
	}
}
void AArchVizController::CompleteBuildingFloor() {
	FloorGeneratorActor = nullptr;
}
void AArchVizController::DestroyFloorPreviewActor() {
	if (FloorGeneratorActor && bShouldEditFloorLocationUnderCursor) {
		FloorGeneratorActor->Destroy();
		FloorGeneratorActor = nullptr;
	}
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
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button to Place the Floor at desired location."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	if (FloorGeneratorActor) {
		bShouldEditFloorLocationUnderCursor = true;
	}
}

//Roof Construction
void AArchVizController::SetupRoofConstructionInputs() {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		RoofConstructionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforRoofConstruction = NewObject<UInputAction>(this);
		ClickforRoofConstruction->ValueType = EInputActionValueType::Boolean;
		RoofConstructionIMC->MapKey(ClickforRoofConstruction, EKeys::MiddleMouseButton);

		UInputAction* ClickforStartingRoofConstruction = NewObject<UInputAction>(this);
		ClickforStartingRoofConstruction->ValueType = EInputActionValueType::Boolean;
		RoofConstructionIMC->MapKey(ClickforStartingRoofConstruction, EKeys::LeftMouseButton);

		UInputAction* ReleaseClickforCompletingRoofConstruction = NewObject<UInputAction>(this);
		ReleaseClickforCompletingRoofConstruction->ValueType = EInputActionValueType::Boolean;
		RoofConstructionIMC->MapKey(ReleaseClickforCompletingRoofConstruction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforRoofConstruction, ETriggerEvent::Completed, this, &AArchVizController::GenerateRoofOnClick);
		EnhancedInputComponent->BindAction(ClickforStartingRoofConstruction, ETriggerEvent::Triggered, this, &AArchVizController::StartBuildingRoof);
		EnhancedInputComponent->BindAction(ReleaseClickforCompletingRoofConstruction, ETriggerEvent::Completed, this, &AArchVizController::CompleteBuildingRoof);
	}
}
void AArchVizController::GenerateRoofOnClick()
{
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	FVector HitLocation = HitResult.Location;
	HitLocation.Z += 100;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

	int8 WallHitCount{};
	FVector WallLocationX{};
	FVector WallLocationY{};
	FVector WallLocation_X{};
	FVector WallLocation_Y{};
	AWallGenerator* TempWallActorX{};
	AWallGenerator* TempWallActor_X{};
	AWallGenerator* TempWallActorY{};
	AWallGenerator* TempWallActor_Y{};

	if (GetWorld()->LineTraceSingleByChannel(HitResult, HitLocation, HitLocation + FVector(1, 0, 0) * 10000, ECC_Visibility, TraceParams)) {
		if(Cast<AWallGenerator>(HitResult.GetActor())) {
			++WallHitCount;
			WallLocationX = HitResult.Location;
			TempWallActorX = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}
	if (GetWorld()->LineTraceSingleByChannel(HitResult, HitLocation, HitLocation + FVector(-1, 0, 0) * 10000, ECC_Visibility, TraceParams)) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			++WallHitCount;
			WallLocation_X = HitResult.Location;
			TempWallActor_X = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}
	if (GetWorld()->LineTraceSingleByChannel(HitResult, HitLocation, HitLocation + FVector(0, 1, 0) * 10000, ECC_Visibility, TraceParams)) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			++WallHitCount;
			WallLocationY = HitResult.Location;
			TempWallActorY = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}
	if (GetWorld()->LineTraceSingleByChannel(HitResult, HitLocation, HitLocation + FVector(0, -1, 0) * 10000, ECC_Visibility, TraceParams)) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			++WallHitCount;
			WallLocation_Y = HitResult.Location;
			TempWallActor_Y = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}

	if (WallHitCount == 4) {
		if((TempWallActorX->HeightOfWall==TempWallActor_X->HeightOfWall) && (TempWallActorX->HeightOfWall == TempWallActorY->HeightOfWall) && TempWallActorX->HeightOfWall == TempWallActor_Y->HeightOfWall) {

			float RoofLength = FMath::Abs(WallLocationX.X - WallLocation_X.X) + (TempWallActorX->WallStaticMesh->GetBounds().GetBox().GetSize().Y * 2);
			float RoofWidth = FMath::Abs(WallLocationY.Y - WallLocation_Y.Y) + (TempWallActorX->WallStaticMesh->GetBounds().GetBox().GetSize().Y * 2);
			float RoofHeight = TempWallActorX->WallStaticMesh->GetBounds().GetBox().GetSize().Y;

			float RoofLocationZ = TempWallActorX->GetActorLocation().Z + TempWallActorX->HeightOfWall;
			FVector RoofLocation = { (WallLocationX.X + WallLocation_X.X) / 2, (WallLocationY.Y + WallLocation_Y.Y) / 2, RoofLocationZ };

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (RoofGeneratorActorRef) {
				RoofGeneratorActor = GetWorld()->SpawnActor<ARoofGenerator>(RoofGeneratorActorRef, RoofLocation, FRotator::ZeroRotator, SpawnParams);
				RoofGeneratorActor->RoofMeasurements = { RoofLength, RoofWidth, RoofHeight };;
				RoofGeneratorActor->GenerateRoof(RoofGeneratorActor->RoofMeasurements);
			}
		}
		else {
			HomeWidget->DisplayMsgTxt->SetText(FText::FromString("The height of all walls in a house must be same to build a roof."));
			HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
		}
	}
	else {
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("There must be 4 walls to build a roof by Middle Mouse Button."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	}
}
void AArchVizController::StartBuildingRoof(){
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if(Cast<AWallGenerator>(HitResult.GetActor())) {
		AWallGenerator* TempWallActor = Cast<AWallGenerator>(HitResult.GetActor());

		if (!RoofGeneratorActor) {
			StartRoofLocation = HitResult.Location;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (RoofGeneratorActorRef) {
				RoofGeneratorActor = GetWorld()->SpawnActor<ARoofGenerator>(RoofGeneratorActorRef, StartRoofLocation, FRotator::ZeroRotator, SpawnParams);
			}
		}
		else {
			FVector CurrentRoofLocation = HitResult.Location;

			if (RoofGeneratorActor) {
				float RoofLength = FMath::Abs(CurrentRoofLocation.X - StartRoofLocation.X);
				float RoofWidth = FMath::Abs(CurrentRoofLocation.Y - StartRoofLocation.Y);
				float RoofHeight = TempWallActor->WallStaticMesh->GetBounds().GetBox().GetSize().Y;

				RoofGeneratorActor->SetActorLocation(FVector((StartRoofLocation.X + CurrentRoofLocation.X) / 2, (StartRoofLocation.Y + CurrentRoofLocation.Y) / 2, TempWallActor->GetActorLocation().Z + TempWallActor->HeightOfWall));
				RoofGeneratorActor->GenerateRoof({ RoofLength, RoofWidth, RoofHeight });
				SnapActor(12.5);
			}
		}
	}
}
void AArchVizController::CompleteBuildingRoof(){
	RoofGeneratorActor = nullptr;
}

// Roof Bind Function
void AArchVizController::OnRoofLocationXValueChanged(float InValue) {
	if (RoofGeneratorActor) {
		FVector Location = RoofGeneratorActor->GetActorLocation();
		RoofGeneratorActor->SetActorLocation(FVector(InValue, Location.Y, Location.Z));
	}
}
void AArchVizController::OnRoofLocationYValueChanged(float InValue) {
	if (RoofGeneratorActor) {
		FVector Location = RoofGeneratorActor->GetActorLocation();
		RoofGeneratorActor->SetActorLocation(FVector(Location.X, InValue, Location.Z));
	}
}
void AArchVizController::OnRoofLocationZValueChanged(float InValue) {
	if (RoofGeneratorActor) {
		FVector Location = RoofGeneratorActor->GetActorLocation();
		RoofGeneratorActor->SetActorLocation(FVector(Location.X, Location.Y, InValue));
	}
}
void AArchVizController::OnRoofDimensionXValueChanged(float InValue) {
	if (RoofGeneratorActor) {
		RoofGeneratorActor->RoofMeasurements.X = InValue;
		RoofGeneratorActor->GenerateRoof(RoofGeneratorActor->RoofMeasurements);
	}
}
void AArchVizController::OnRoofDimensionYValueChanged(float InValue) {
	if (RoofGeneratorActor) {
		RoofGeneratorActor->RoofMeasurements.Y = InValue;
		RoofGeneratorActor->GenerateRoof(RoofGeneratorActor->RoofMeasurements);
	}
}
void AArchVizController::OnRoofDimensionZValueChanged(float InValue){
	if (RoofGeneratorActor) {
		RoofGeneratorActor->RoofMeasurements.Z = InValue;
		RoofGeneratorActor->GenerateRoof(RoofGeneratorActor->RoofMeasurements);
	}
}
void AArchVizController::OnDestroyRoofBtnClicked(){
	HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Press Left Mouse Button to Place the Roof at desired location."));
	HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	if (RoofGeneratorActor) {
		RoofGeneratorActor->Destroy();
		RoofGeneratorActor = nullptr;

		BuildingConstructionWidget->RoofLocationBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->RoofDimensionsBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructionWidget->DestroyRoofBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Material Menu
void AArchVizController::SetupMaterialMenuInputs(){
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		MaterialSelectionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforSelectingAssetToApplyMaterial = NewObject<UInputAction>(this);
		ClickforSelectingAssetToApplyMaterial->ValueType = EInputActionValueType::Boolean;
		MaterialSelectionIMC->MapKey(ClickforSelectingAssetToApplyMaterial, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforSelectingAssetToApplyMaterial, ETriggerEvent::Completed, this, &AArchVizController::SelectAssetOnClickForMaterial);
	}
}
void AArchVizController::SelectAssetOnClickForMaterial() {
	
	RemovePostProcessMaterial();

	RoadGeneratorActor = nullptr;
	WallGeneratorActor = nullptr;
	DoorStaticMeshActor = nullptr;
	FloorGeneratorActor = nullptr;
	RoofGeneratorActor = nullptr;

	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (Cast<ARoadGenerator>(HitResult.GetActor())) {
		MaterialSelectionWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		MaterialSelectionWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

		RoadGeneratorActor = Cast<ARoadGenerator>(HitResult.GetActor());
		RoadGeneratorActor->RoadProceduralMeshComponent->SetRenderCustomDepth(true);
		RoadGeneratorActor->RoadProceduralMeshComponent->CustomDepthStencilValue = 2.0;
	}
	else if (Cast<AWallGenerator>(HitResult.GetActor())) {
		MaterialSelectionWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		MaterialSelectionWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

		WallGeneratorActor = Cast<AWallGenerator>(HitResult.GetActor());
		bIsInBuildingEditorMode = true;

		for (int i{}; i < WallGeneratorActor->WallStaticMeshComponentsArr.Num(); i++) {
			if (WallGeneratorActor->HeightOfWall == WallGeneratorActor->WallStaticMeshComponentsArr[i]->GetStaticMesh()->GetBounds().GetBox().GetSize().Z)
			{
				WallGeneratorActor->WallStaticMeshComponentsArr[i]->SetRenderCustomDepth(true);
				WallGeneratorActor->WallStaticMeshComponentsArr[i]->CustomDepthStencilValue = 2.0;
			}
		}
		for (auto& WallProcedural : WallGeneratorActor->WallActorMap) {
			WallProcedural.Value.ProceduralMeshComponent->SetRenderCustomDepth(true);
			WallProcedural.Value.ProceduralMeshComponent->CustomDepthStencilValue = 2.0;
		}
	}
	else if (Cast<AFloorGenerator>(HitResult.GetActor())) {
		MaterialSelectionWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		MaterialSelectionWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

		FloorGeneratorActor = Cast<AFloorGenerator>(HitResult.GetActor());
		FloorGeneratorActor->FloorProceduralMeshComponent->SetRenderCustomDepth(true);
		FloorGeneratorActor->FloorProceduralMeshComponent->CustomDepthStencilValue = 2.0;
	}
	else if (Cast<ARoofGenerator>(HitResult.GetActor())) {
		MaterialSelectionWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		MaterialSelectionWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

		RoofGeneratorActor = Cast<ARoofGenerator>(HitResult.GetActor());
		RoofGeneratorActor->RoofProceduralMeshComponent->SetRenderCustomDepth(true);
		RoofGeneratorActor->RoofProceduralMeshComponent->CustomDepthStencilValue = 2.0;
	}
	else {
		MaterialSelectionWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialSelectionWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AArchVizController::ApplyMaterialToRoad(const FRoadMaterialData& RoadMaterialData) {
	if(RoadGeneratorActor){
		RoadGeneratorActor->ApplyMaterialToRoadActor(RoadMaterialData.Material);
	}
}
void AArchVizController::ApplyMaterialToBuildingComponents(const FBuildingMaterialData& BuildingMaterialData) {
	if (WallGeneratorActor) {
		WallGeneratorActor->ApplyMaterialToWallActor(BuildingMaterialData.Material);
	}
	else if (FloorGeneratorActor) {
		FloorGeneratorActor->ApplyMaterialToFloorActor(BuildingMaterialData.Material);
	}
	else if (RoofGeneratorActor) {
		RoofGeneratorActor->ApplyMaterialToRoofActor(BuildingMaterialData.Material);
	}
}

// Interior Design
void AArchVizController::SetupInteriorDesignInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		InteriorDesignIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickToPlaceInterior = NewObject<UInputAction>(this);
		ClickToPlaceInterior->ValueType = EInputActionValueType::Boolean;
		InteriorDesignIMC->MapKey(ClickToPlaceInterior, EKeys::LeftMouseButton);

		UInputAction* RotateAction = NewObject<UInputAction>(this);
		RotateAction->ValueType = EInputActionValueType::Boolean;
		InteriorDesignIMC->MapKey(RotateAction, EKeys::R);

		UInputAction* DestroyAction = NewObject<UInputAction>(this);
		DestroyAction->ValueType = EInputActionValueType::Boolean;
		InteriorDesignIMC->MapKey(DestroyAction, EKeys::Delete);

		EnhancedInputComponent->BindAction(ClickToPlaceInterior, ETriggerEvent::Completed, this, &AArchVizController::PlaceInteriorOnClick);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &AArchVizController::RotateInterior);
		EnhancedInputComponent->BindAction(DestroyAction, ETriggerEvent::Completed, this, &AArchVizController::DestroyInterior);
	}
}
void AArchVizController::PlaceInteriorOnClick() {
	if(IsValid(InteriorStaticMesh)) {
		FVector InteriorSpawnLocation{};
		FRotator InteriorSpawnRotation{};
		if (InteriorDesignActor) {
			InteriorSpawnLocation = InteriorDesignActor->GetActorLocation();
			InteriorSpawnRotation = InteriorDesignActor->GetActorRotation();

			InteriorDesignActor->Destroy();
			InteriorDesignActor = nullptr;
		}

		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

		if (BuildingAssetSelectedController == EBuildingAsset::Wall) {
			if (Cast<AWallGenerator>(HitResult.GetActor())) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), InteriorSpawnLocation, InteriorSpawnRotation, SpawnParams);
				InteriorDesignActor->BuildingAssetForInterior = EBuildingAsset::Wall;
				InteriorDesignActor->SetInteriorStaticMesh(InteriorStaticMesh);

				InteriorStaticMesh = nullptr;
				InteriorDesignActor = nullptr;
			}
			else {
				HomeWidget->DisplayMsgTxt->SetText(FText::FromString("The selected Interior can only be placed on Wall."));
				HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
			}
		}
		else if (BuildingAssetSelectedController == EBuildingAsset::Floor) {
			if (Cast<AFloorGenerator>(HitResult.GetActor())) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), InteriorSpawnLocation, InteriorSpawnRotation, SpawnParams);
				InteriorDesignActor->BuildingAssetForInterior = EBuildingAsset::Floor;
				InteriorDesignActor->SetInteriorStaticMesh(InteriorStaticMesh);

				InteriorStaticMesh = nullptr;
				InteriorDesignActor = nullptr;
			}
			else {
				HomeWidget->DisplayMsgTxt->SetText(FText::FromString("The selected Interior can only be placed on Floor."));
				HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
			}
		}
		else if (BuildingAssetSelectedController == EBuildingAsset::Roof) {
			if (Cast<ARoofGenerator>(HitResult.GetActor())) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), InteriorSpawnLocation, InteriorSpawnRotation, SpawnParams);
				InteriorDesignActor->BuildingAssetForInterior = EBuildingAsset::Roof;
				InteriorDesignActor->SetInteriorStaticMesh(InteriorStaticMesh);

				InteriorStaticMesh = nullptr;
				InteriorDesignActor = nullptr;
			}
			else {
				HomeWidget->DisplayMsgTxt->SetText(FText::FromString("The selected Interior can only be placed on Roof."));
				HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
			}
		}
	}
	else {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;

		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			if (Cast<AInteriorDesign>(HitResult.GetActor())) {
				InteriorDesignActor = Cast<AInteriorDesign>(HitResult.GetActor());
				InteriorDesignActor->InteriorStaticMeshComponent->SetRenderCustomDepth(true);
				InteriorDesignActor->InteriorStaticMeshComponent->CustomDepthStencilValue = 2.0;

				InteriorStaticMesh = InteriorDesignActor->InteriorStaticMesh;
			}
		}
	}
}
void AArchVizController::RotateInterior(){
	if (InteriorDesignActor && BuildingAssetSelectedController != EBuildingAsset::Wall) {
		InteriorDesignActor->SetActorRotation(FRotator(InteriorDesignActor->GetActorRotation().Pitch, InteriorDesignActor->GetActorRotation().Yaw + 90, InteriorDesignActor->GetActorRotation().Roll));
	}
}
void AArchVizController::SetInterior(const FInteriorDesignData& InteriorDesignData) {
	if(InteriorDesignData.InteriorStaticMesh) {
		InteriorStaticMesh = InteriorDesignData.InteriorStaticMesh;
	}
	BuildingAssetSelectedController = InteriorDesignData.BuildingAsset;
}
void AArchVizController::DestroyInteriorPreviewActor() {
	if (InteriorDesignActor) {
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}
}
void AArchVizController::DestroyInterior() {
	if (InteriorDesignActor) {
		InteriorStaticMesh = nullptr;
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}
}

// Save / Load
void AArchVizController::ShowSaveMenu() {
	SaveLoadWidget->SaveMenu->SetVisibility(ESlateVisibility::Visible);
	SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->CloseSaveMenuBtn->SetVisibility(ESlateVisibility::Visible);
	SaveLoadWidget->SlotNameTxt->SetText(FText::FromString(""));
}
void AArchVizController::ShowLoadMenu() {
	SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Visible);
	SaveLoadWidget->SaveMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->CloseLoadMenuBtn->SetVisibility(ESlateVisibility::Visible);
	SaveLoadWidget->SlotTypeMenu->SetVisibility(ESlateVisibility::Hidden);


	LoadSlotList();
}
void AArchVizController::HideSaveMenu() {
	SaveLoadWidget->SaveMenu->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::HideLoadMenu() {
	SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Hidden);
}
bool AArchVizController::CheckFileExists(const FString& FileName) {
	IFileManager& FileManager = IFileManager::Get();

	FString FilePath = FPaths::ProjectSavedDir() + "SaveGames/" + FileName + ".sav";
	return FileManager.FileExists(*FilePath);
}
void AArchVizController::SaveTemplate() {
	FText SlotText = SaveLoadWidget->SlotNameTxt->GetText();

	if (!SlotText.IsEmpty())
	{
		if (!SlotText.ToString().StartsWith(TEXT("Template"), 8, ESearchCase::CaseSensitive)) {
			if (CheckFileExists(SlotText.ToString())) {
				SaveLoadWidget->ConfirmationBox->SetVisibility(ESlateVisibility::Visible);
				SaveLoadWidget->CloseSaveMenuBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
				SaveLoadWidget->AlreadyExistsMsgTxt->SetText(FText::FromString("Slot named \"" + SlotText.ToString() + "\" is already present."));
			}
			else {
				HideSaveMenu();

				UArchVizExplorerSaveGame* SaveArchVizInstance = Cast<UArchVizExplorerSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizExplorerSaveGame::StaticClass()));

				TArray<AActor*> RoadActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoadGeneratorActorRef, RoadActors);

				for (AActor* Actor : RoadActors)
				{
					ARoadGenerator* RoadActor = Cast<ARoadGenerator>(Actor);
					if (RoadActor)
					{
						FRoadSaveData RoadData;
						RoadData.RoadTransform = RoadActor->GetActorTransform();
						RoadData.RoadMaterial = RoadActor->RoadMaterial;
						RoadData.RoadDimensions = RoadActor->RoadMeasurements;

						SaveArchVizInstance->RoadActorArray.Add(RoadData);
					}
				}

				TArray<AActor*> WallActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), WallGeneratorActorRef, WallActors);

				for (AActor* Actor : WallActors)
				{
					AWallGenerator* WallActor = Cast<AWallGenerator>(Actor);
					if (WallActor)
					{
						FWallSaveData WallData;
						WallData.WallTransform = WallActor->GetActorTransform();
						WallData.WallStaticMesh = WallActor->WallStaticMesh;
						WallData.WallMaterial = WallActor->WallMaterial;
						WallData.NoOfSegments = WallActor->SegmentsNo;
						WallData.WallActorMap = WallActor->WallActorMap;

						SaveArchVizInstance->WallActorArray.Add(WallData);
					}
				}

				TArray<AActor*> FloorActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), FloorGeneratorActorRef, FloorActors);

				for (AActor* Actor : FloorActors)
				{
					AFloorGenerator* FloorActor = Cast<AFloorGenerator>(Actor);
					if (FloorActor)
					{
						FFloorSaveData FloorData;
						FloorData.FloorTransform = FloorActor->GetActorTransform();
						FloorData.FloorMaterial = FloorActor->FloorMaterial;
						FloorData.FloorDimensions = FloorActor->FloorMeasurements;

						SaveArchVizInstance->FloorActorArray.Add(FloorData);
					}
				}

				TArray<AActor*> RoofActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoofGeneratorActorRef, RoofActors);

				for (AActor* Actor : RoofActors)
				{
					ARoofGenerator* RoofActor = Cast<ARoofGenerator>(Actor);
					if (RoofActor)
					{
						FRoofSaveData RoofData;
						RoofData.RoofTransform = RoofActor->GetActorTransform();
						RoofData.RoofMaterial = RoofActor->RoofMaterial;
						RoofData.RoofDimensions = RoofActor->RoofMeasurements;

						SaveArchVizInstance->RoofActorArray.Add(RoofData);
					}
				}

				TArray<AActor*> InteriorActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorDesign::StaticClass(), InteriorActors);

				for (AActor* Actor : InteriorActors)
				{
					AInteriorDesign* InteriorActor = Cast<AInteriorDesign>(Actor);
					if (InteriorActor)
					{
						FInteriorSaveData InteriorData;
						InteriorData.InteriorTransform = InteriorActor->GetActorTransform();
						InteriorData.BuildingAssetForInterior = InteriorActor->BuildingAssetForInterior;
						InteriorData.InteriorMesh = InteriorActor->InteriorStaticMesh;

						SaveArchVizInstance->InteriorActorArray.Add(InteriorData);
					}
				}

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Saved");

				UGameplayStatics::SaveGameToSlot(SaveArchVizInstance, SlotText.ToString(), 0);
			}
		}
		else {
			HomeWidget->DisplayMsgTxt->SetText(FText::FromString("You cannot give Slot Name starting with \"Template\"."));
			HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
			SaveLoadWidget->NewSlotNameTxt->SetText(FText::FromString(""));
		}
	}
	else {
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Slot Name cannot be empty."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	}
}
void AArchVizController::LoadSlotWithGivenName(const FText& SlotName) {
	UArchVizExplorerSaveGame* LoadGameInstance = Cast<UArchVizExplorerSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName.ToString(), 0));

	if (LoadGameInstance)
	{
		HideLoadMenu();

		ClearViewportBeforeLoad();
		for (const FRoadSaveData& RoadData : LoadGameInstance->RoadActorArray)
		{
			ARoadGenerator* RoadActor = GetWorld()->SpawnActor<ARoadGenerator>(RoadGeneratorActorRef, RoadData.RoadTransform);
			RoadActor->ApplyMaterialToRoadActor(RoadData.RoadMaterial);
			RoadActor->GenerateRoad(RoadData.RoadDimensions);
		}
		for (const FWallSaveData& WallData : LoadGameInstance->WallActorArray)
		{
			AWallGenerator* WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallGeneratorActorRef, WallData.WallTransform);
			WallActor->WallStaticMesh = WallData.WallStaticMesh;
			WallActor->WallActorMap = WallData.WallActorMap;
			WallActor->GenerateWall(WallData.NoOfSegments);
			WallActor->ApplyMaterialToWallActor(WallData.WallMaterial);
		}
		for (const FFloorSaveData& FloorData : LoadGameInstance->FloorActorArray)
		{
			AFloorGenerator* FloorActor = GetWorld()->SpawnActor<AFloorGenerator>(FloorGeneratorActorRef, FloorData.FloorTransform);
			FloorActor->GenerateFloor(FloorData.FloorDimensions);
			FloorActor->ApplyMaterialToFloorActor(FloorData.FloorMaterial);
		}
		for (const FRoofSaveData& RoofData : LoadGameInstance->RoofActorArray)
		{
			ARoofGenerator* RoofActor = GetWorld()->SpawnActor<ARoofGenerator>(RoofGeneratorActorRef, RoofData.RoofTransform);
			RoofActor->GenerateRoof(RoofData.RoofDimensions);
			RoofActor->ApplyMaterialToRoofActor(RoofData.RoofMaterial);
		}
		for (const FInteriorSaveData& InteriorData : LoadGameInstance->InteriorActorArray) {
			AInteriorDesign* InteriorActor = GetWorld()->SpawnActor<AInteriorDesign>(AInteriorDesign::StaticClass(), InteriorData.InteriorTransform);
			InteriorActor->BuildingAssetForInterior = InteriorData.BuildingAssetForInterior;
			InteriorActor->InteriorStaticMesh = InteriorData.InteriorMesh;
			InteriorActor->SetInteriorStaticMesh(InteriorActor->InteriorStaticMesh);
		}
		CurrentSelectedMode = EModeSelected::ViewMode;
		HomeWidget->ModeSelectionDropdown->SetSelectedOption(FString("View Mode"));
		OnModeSelectionChanged(FString("View Mode"), ESelectInfo::Direct);
	}
}
void AArchVizController::ClearViewportBeforeLoad() {
	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoadGeneratorActorRef, Actors);
	for (auto& RoadActor : Actors) {
		RoadActor->Destroy();
		RoadActor = nullptr;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WallGeneratorActorRef, Actors);
	for (auto& WallActor : Actors) {
		WallActor->Destroy();
		WallActor = nullptr;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FloorGeneratorActorRef, Actors);
	for (auto& FloorActor : Actors) {
		FloorActor->Destroy();
		FloorActor = nullptr;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoofGeneratorActorRef, Actors);
	for (auto& RoofActor : Actors) {
		RoofActor->Destroy();
		RoofActor = nullptr;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorDesign::StaticClass(), Actors);
	for (auto& InteriorActor : Actors) {
		InteriorActor->Destroy();
		InteriorActor = nullptr;
	}

}
void AArchVizController::LoadSlotList(){
	SaveLoadWidget->SlotListScrollBox->ClearChildren();

	FString SavedDirPath = FPaths::ProjectSavedDir();
	SavedDirPath.Append("/SaveGames");
	
	TArray<FString> SlotFiles = FindFiles(SavedDirPath, ".sav");
	for (FString SlotFile : SlotFiles) {
		if (SlotFile.StartsWith(TEXT("Template"), 8, ESearchCase::CaseSensitive)) {
			SlotFiles.RemoveSingleSwap(SlotFile, true);
		}
	}

	if (SlotFiles.Num() == 0) {
		SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Hidden);
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("There are no Slots currently. Create one to Load it."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	}
	else {
		SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Visible);
		for (int i = 0; i < SlotFiles.Num(); ++i) {
			if (SlotListWidgetClassRef) {
				SlotListWidget = CreateWidget<USlotListWidget>(this, SlotListWidgetClassRef);
			}
			if (SlotListWidget) {
				SlotListWidget->OnTemplateSlotPressed.BindUFunction(this, "LoadSlotWithGivenName");
				SlotListWidget->OnDeleteTemplateSlotPressed.BindUFunction(this, "DeleteSlotWithGivenName");
				SlotListWidget->OnEditTemplateSlotNamePressed.BindUFunction(this, "ShowRenameMenu");
				SlotListWidget->LoadSlotName->SetText(FText::FromString(SlotFiles[i].LeftChop(4)));
				SaveLoadWidget->SlotListScrollBox->AddChild(SlotListWidget);
			}
		}
	}
}
TArray<FString> AArchVizController::FindFiles(FString FilePath, FString FileExtension) {
	TArray<FString> FileList;
	IFileManager& FileManager = IFileManager::Get();

	if (!FilePath.EndsWith("/")) {
		FilePath += "/";
	}

	FString CompletePath = FilePath + TEXT("*") + FileExtension;
	FileManager.FindFiles(FileList, *CompletePath, true, false);
	return FileList;
}
void AArchVizController::DeleteSlotWithGivenName(const FText& SlotName) {
	UGameplayStatics::DeleteGameInSlot(SlotName.ToString(), 0);
	LoadSlotList();
}
void AArchVizController::ReplaceSlot() {
	UArchVizExplorerSaveGame* SaveArchVizInstance = Cast<UArchVizExplorerSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizExplorerSaveGame::StaticClass()));

	TArray<AActor*> RoadActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoadGeneratorActorRef, RoadActors);

	for (AActor* Actor : RoadActors)
	{
		ARoadGenerator* RoadActor = Cast<ARoadGenerator>(Actor);
		if (RoadActor)
		{
			FRoadSaveData RoadData;
			RoadData.RoadTransform = RoadActor->GetActorTransform();
			RoadData.RoadMaterial = RoadActor->RoadMaterial;
			RoadData.RoadDimensions = RoadActor->RoadMeasurements;

			SaveArchVizInstance->RoadActorArray.Add(RoadData);
		}
	}

	TArray<AActor*> WallActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WallGeneratorActorRef, WallActors);

	for (AActor* Actor : WallActors)
	{
		AWallGenerator* WallActor = Cast<AWallGenerator>(Actor);
		if (WallActor)
		{
			FWallSaveData WallData;
			WallData.WallTransform = WallActor->GetActorTransform();
			WallData.WallStaticMesh = WallActor->WallStaticMesh;
			WallData.WallMaterial = WallActor->WallMaterial;
			WallData.NoOfSegments = WallActor->SegmentsNo;
			WallData.WallActorMap = WallActor->WallActorMap;

			SaveArchVizInstance->WallActorArray.Add(WallData);
		}
	}

	TArray<AActor*> FloorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FloorGeneratorActorRef, FloorActors);

	for (AActor* Actor : FloorActors)
	{
		AFloorGenerator* FloorActor = Cast<AFloorGenerator>(Actor);
		if (FloorActor)
		{
			FFloorSaveData FloorData;
			FloorData.FloorTransform = FloorActor->GetActorTransform();
			FloorData.FloorMaterial = FloorActor->FloorMaterial;
			FloorData.FloorDimensions = FloorActor->FloorMeasurements;

			SaveArchVizInstance->FloorActorArray.Add(FloorData);
		}
	}

	TArray<AActor*> RoofActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoofGeneratorActorRef, RoofActors);

	for (AActor* Actor : RoofActors)
	{
		ARoofGenerator* RoofActor = Cast<ARoofGenerator>(Actor);
		if (RoofActor)
		{
			FRoofSaveData RoofData;
			RoofData.RoofTransform = RoofActor->GetActorTransform();
			RoofData.RoofMaterial = RoofActor->RoofMaterial;
			RoofData.RoofDimensions = RoofActor->RoofMeasurements;

			SaveArchVizInstance->RoofActorArray.Add(RoofData);
		}
	}

	TArray<AActor*> InteriorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorDesign::StaticClass(), InteriorActors);

	for (AActor* Actor : InteriorActors)
	{
		AInteriorDesign* InteriorActor = Cast<AInteriorDesign>(Actor);
		if (InteriorActor)
		{
			FInteriorSaveData InteriorData;
			InteriorData.InteriorTransform = InteriorActor->GetActorTransform();
			InteriorData.BuildingAssetForInterior = InteriorActor->BuildingAssetForInterior;
			InteriorData.InteriorMesh = InteriorActor->InteriorStaticMesh;

			SaveArchVizInstance->InteriorActorArray.Add(InteriorData);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Saved");
	UGameplayStatics::SaveGameToSlot(SaveArchVizInstance, SaveLoadWidget->SlotNameTxt->GetText().ToString(), 0);

	SaveLoadWidget->ConfirmationBox->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->SaveMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->SlotTypeMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->CloseSaveMenuBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
}
void AArchVizController::RewriteSlotName() {
	SaveLoadWidget->ConfirmationBox->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->SlotNameTxt->SetText(FText::FromString(""));
	SaveLoadWidget->CloseSaveMenuBtn->SetVisibility(ESlateVisibility::Visible);
}
void AArchVizController::ShowRenameMenu(const FText& FileName) {
	SaveLoadWidget->RenameMenu->SetVisibility(ESlateVisibility::Visible);
	SaveLoadWidget->CloseLoadMenuBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
	SaveLoadWidget->NewSlotNameTxt->SetText(FText::FromString(""));
	SelectedSlotName = FileName.ToString();

	SaveLoadWidget->RenamingSlotMsgTxt->SetText(FText::FromString("Renaming \"" + SelectedSlotName + "\" Slot."));
}
void AArchVizController::HideRenameMenu() {
	SaveLoadWidget->RenameMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->CloseLoadMenuBtn->SetVisibility(ESlateVisibility::Visible);
}
void AArchVizController::RenameSlot() {
	IFileManager& FileManager = IFileManager::Get();
	FString SelectedSlotFilePath = FPaths::ProjectSavedDir() + "SaveGames/" + SelectedSlotName + ".sav";
	if (SaveLoadWidget->NewSlotNameTxt->GetText().IsEmpty()) {
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("The Slot Name cannot be empty."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	}
	else
	{
		if (FileManager.FileExists(*SelectedSlotFilePath)) {

			FString NewSlotName = SaveLoadWidget->NewSlotNameTxt->GetText().ToString();
			if (SelectedSlotName == NewSlotName) {
				HomeWidget->DisplayMsgTxt->SetText(FText::FromString("This Slot's name is already \"" + SelectedSlotName + "\"."));
				HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
				SaveLoadWidget->NewSlotNameTxt->SetText(FText::FromString(""));
			}
			else {

				FString NewSlotFilePath = FPaths::ProjectSavedDir() + "SaveGames/" + NewSlotName + ".sav";
				FileManager.Move(*NewSlotFilePath, *SelectedSlotFilePath, true);

				HomeWidget->DisplayMsgTxt->SetText(FText::FromString("Slot \"" + SelectedSlotName + "\" is renamed to \"" + NewSlotName + "\"."));
				HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);

				SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Hidden);
				SaveLoadWidget->RenameMenu->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else {
			HomeWidget->DisplayMsgTxt->SetText(FText::FromString("No Slot with \"" + SelectedSlotName + "\" exists."));
			HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
		}
	}
}

void AArchVizController::ShowSlotTypeMenu() {
	SaveLoadWidget->SlotTypeMenu->SetVisibility(ESlateVisibility::Visible);
	SaveLoadWidget->LoadMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->SaveMenu->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::HideSlotTypeMenu() {
	SaveLoadWidget->SlotTypeMenu->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::LoadExistingSlotList() {
	SaveLoadWidget->SaveInExisitingSlotListScrollBox->ClearChildren();

	FString SavedDirPath = FPaths::ProjectSavedDir();
	SavedDirPath.Append("/SaveGames");

	TArray<FString> SlotFiles = FindFiles(SavedDirPath, ".sav");
	for (FString SlotFile : SlotFiles) {
		if (SlotFile.StartsWith(TEXT("Template"), 8, ESearchCase::CaseSensitive)) {
			SlotFiles.RemoveSingleSwap(SlotFile, true);
		}
	}

	if (SlotFiles.Num() == 0) {
		HomeWidget->DisplayMsgTxt->SetText(FText::FromString("There are no Slots currently. Create one to Load it."));
		HomeWidget->PlayAnimation(HomeWidget->DisplayMsgAnim);
	}
	else {
		for (int i = 0; i < SlotFiles.Num(); ++i) {
			if (SlotListWidgetClassRef) {
				SaveInExisitingSlotWidget = CreateWidget<USaveInExisitingSlotWidget>(this, SaveInExisitingSlotWidgetClassRef);
			}
			if (SaveInExisitingSlotWidget) {
				SaveInExisitingSlotWidget->OnProjectSlotPressed.BindUFunction(this, "SaveInExistingSlot");
				SaveInExisitingSlotWidget->LoadSlotName->SetText(FText::FromString(SlotFiles[i].LeftChop(4)));
				SaveLoadWidget->SaveInExisitingSlotListScrollBox->AddChild(SaveInExisitingSlotWidget);
			}
		}
	}
}
void AArchVizController::ShowExistingSlotMenu() {
	SaveLoadWidget->SaveInExisitingSlotMenu->SetVisibility(ESlateVisibility::Visible);

	LoadExistingSlotList();
}
void AArchVizController::SaveInExistingSlot(const FText& SlotName) {
	//FText SlotText = SaveLoadWidget->SlotNameTxt->GetText();

	UArchVizExplorerSaveGame* SaveArchVizInstance = Cast<UArchVizExplorerSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizExplorerSaveGame::StaticClass()));

	TArray<AActor*> RoadActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoadGeneratorActorRef, RoadActors);
	for (AActor* Actor : RoadActors)
	{
		ARoadGenerator* RoadActor = Cast<ARoadGenerator>(Actor);
		if (RoadActor)
		{
			FRoadSaveData RoadData;
			RoadData.RoadTransform = RoadActor->GetActorTransform();
			RoadData.RoadMaterial = RoadActor->RoadMaterial;
			RoadData.RoadDimensions = RoadActor->RoadMeasurements;

			SaveArchVizInstance->RoadActorArray.Add(RoadData);
		}
	}

	TArray<AActor*> WallActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WallGeneratorActorRef, WallActors);
	for (AActor* Actor : WallActors)
	{
		AWallGenerator* WallActor = Cast<AWallGenerator>(Actor);
		if (WallActor)
		{
			FWallSaveData WallData;
			WallData.WallTransform = WallActor->GetActorTransform();
			WallData.WallStaticMesh = WallActor->WallStaticMesh;
			WallData.WallMaterial = WallActor->WallMaterial;
			WallData.NoOfSegments = WallActor->SegmentsNo;
			WallData.WallActorMap = WallActor->WallActorMap;

			SaveArchVizInstance->WallActorArray.Add(WallData);
		}
	}

	TArray<AActor*> FloorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FloorGeneratorActorRef, FloorActors);
	for (AActor* Actor : FloorActors)
	{
		AFloorGenerator* FloorActor = Cast<AFloorGenerator>(Actor);
		if (FloorActor)
		{
			FFloorSaveData FloorData;
			FloorData.FloorTransform = FloorActor->GetActorTransform();
			FloorData.FloorMaterial = FloorActor->FloorMaterial;
			FloorData.FloorDimensions = FloorActor->FloorMeasurements;

			SaveArchVizInstance->FloorActorArray.Add(FloorData);
		}
	}

	TArray<AActor*> RoofActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoofGeneratorActorRef, RoofActors);
	for (AActor* Actor : RoofActors)
	{
		ARoofGenerator* RoofActor = Cast<ARoofGenerator>(Actor);
		if (RoofActor)
		{
			FRoofSaveData RoofData;
			RoofData.RoofTransform = RoofActor->GetActorTransform();
			RoofData.RoofMaterial = RoofActor->RoofMaterial;
			RoofData.RoofDimensions = RoofActor->RoofMeasurements;

			SaveArchVizInstance->RoofActorArray.Add(RoofData);
		}
	}

	TArray<AActor*> InteriorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorDesign::StaticClass(), InteriorActors);
	for (AActor* Actor : InteriorActors)
	{
		AInteriorDesign* InteriorActor = Cast<AInteriorDesign>(Actor);
		if (InteriorActor)
		{
			FInteriorSaveData InteriorData;
			InteriorData.InteriorTransform = InteriorActor->GetActorTransform();
			InteriorData.BuildingAssetForInterior = InteriorActor->BuildingAssetForInterior;
			InteriorData.InteriorMesh = InteriorActor->InteriorStaticMesh;

			SaveArchVizInstance->InteriorActorArray.Add(InteriorData);
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveArchVizInstance, SlotName.ToString(), 0);


	SaveLoadWidget->SaveInExisitingSlotMenu->SetVisibility(ESlateVisibility::Hidden);
	SaveLoadWidget->SlotTypeMenu->SetVisibility(ESlateVisibility::Hidden);
}
void AArchVizController::CloseExistingSlotMenu() {
	SaveLoadWidget->SaveInExisitingSlotMenu->SetVisibility(ESlateVisibility::Hidden);
}