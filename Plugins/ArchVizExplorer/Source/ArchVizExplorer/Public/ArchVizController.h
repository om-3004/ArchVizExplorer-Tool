// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProceduralMeshComponent.h"
#include "RoadGenerator.h"

#include "Widgets/HomeWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingConstructionWidget.h"

#include "WallGenerator.h"
#include "FloorGenerator.h"
#include "RoofGenerator.h"

#include "ArchVizController.generated.h"

UENUM(BlueprintType)
enum class EModeSelected : uint8 {
	ViewMode,
	RoadConstruction,
	BuildingConstruction,
	InteriorDesign,
	MaterialSelection,
};

UENUM(BlueprintType)
enum class ERodeMode : uint8 {
	ConstructionMode,
	EditorMode,
};

UENUM(BlueprintType)
enum class EBuildingComponent : uint8 {
	None,
	Wall,
	Door,
	Floor,
	Roof,
};

UENUM(BlueprintType)
enum class EBuildingMode : uint8 {
	ConstructionMode,
	EditorMode,
};

UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController
{
	GENERATED_BODY()

public:
	AArchVizController();

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Home Widget
	UPROPERTY()
	UHomeWidget* HomeWidget;
	UPROPERTY(EditAnywhere, Category = "ArchVizController | WidgetReference")
	TSubclassOf<UHomeWidget> HomeWidgetClassRef;
	UPROPERTY()
	EModeSelected CurrentSelectedMode;
	UPROPERTY()
	ERodeMode CurrentRoadMode;
	UPROPERTY()
	EBuildingComponent CurrentBuildingComponent;
	// Function - Home
	UFUNCTION()
	void UpdateWidget();
	UFUNCTION()
	void UpdateInputMappings();
	UFUNCTION()
	void CreateWidgets();
	UFUNCTION()
	void BindWidgetDelegates();
	UFUNCTION()
	void SetDefaultMode();
	// Widget Bind Function - Home
	UFUNCTION()
	void OnModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// Road Construction
	UPROPERTY()
	URoadConstructionWidget* RoadConstructionWidget;
	UPROPERTY(EditAnywhere, Category = "ArchVizController | WidgetReference")
	TSubclassOf<URoadConstructionWidget> RoadConstructionWidgetClassRef;
	UPROPERTY()
	ARoadGenerator* RoadGeneratorActor;
	UPROPERTY()
	TSubclassOf<ARoadGenerator> RoadGeneratorActorRef;
	UPROPERTY()
	UInputMappingContext* RoadConstructionIMC;
	UPROPERTY(EditDefaultsOnly, Category = "ArchVizController | Road")
	UMaterialInterface* RoadMaterial;
	UPROPERTY()
	FVector RoadDimensions;
	UPROPERTY()
	bool isFirstClick;
	UPROPERTY()
	bool getLocation;
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector EndLocation;
	// Function - Road
	UFUNCTION()
	void SetupRoadConstructionInputs();
	UFUNCTION()
	void GetRoadLocationOnClick();
	UFUNCTION()
	void GenerateRoad();
	UFUNCTION()
	float FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2);
	// Widget Bind Function - Road
	UFUNCTION()
	void OnRoadModeToggleBtnClicked();
	UFUNCTION()
	void GenerateNewRoad();
	UFUNCTION()
	void OnRoadWidthValueChanged(float InValue);
	UFUNCTION()
	void OnRoadLocationXValueChanged(float InValue);
	UFUNCTION()
	void OnRoadLocationYValueChanged(float InValue);

	// Building Construction Widget
	UPROPERTY()
	UBuildingConstructionWidget* BuildingConstructionWidget;
	UPROPERTY(EditAnywhere, Category = "ArchVizController | WidgetReference")
	TSubclassOf<UBuildingConstructionWidget> BuildingConstructionWidgetClassRef;
	UPROPERTY()
	EBuildingMode CurrentBuildingMode;
	UPROPERTY()
	UInputMappingContext* BuildingEditorIMC;
	UPROPERTY()
	bool bIsInBuildingEditorMode;
	UPROPERTY()
	bool bShouldEditWallLocationUnderCursor;
	// Function - Building
	UFUNCTION()
	void UpdateBuildingMappings();
	UFUNCTION()
	void SetupBuildingEditorInputs();
	UFUNCTION()
	void SelectBuildingComponentOnClick();
	// Widget Bind Function - Building
	UFUNCTION()
	void OnBuildingModeToggleBtnClicked();
	UFUNCTION()
	void OnWallLocationXValueChanged(float InValue);
	UFUNCTION()
	void OnWallLocationYValueChanged(float InValue);
	UFUNCTION()
	void OnDestroyWallBtnClicked();
	UFUNCTION()
	void OnDestroyDoorBtnClicked();
	UFUNCTION()
	void OnUpdateWallLocationUnderCursorBtnClicked();

	// Wall Construction
	UPROPERTY()
	AWallGenerator* WallGeneratorActor;
	UPROPERTY(EditDefaultsOnly, Category = "ArchVizController | Wall")
	TSubclassOf<AWallGenerator> WallGeneratorActorRef;
	UPROPERTY()
	UInputMappingContext* WallConstructionIMC;
	UPROPERTY()
	FVector WallLocation;
	UPROPERTY()
	UStaticMesh* TempWallMesh;
	// Function - Wall
	UFUNCTION()
	void SetupWallConstructionInputs();
	UFUNCTION()
	void PreviewWall();
	UFUNCTION()
	void BuildWallAtClick();
	UFUNCTION()
	void RotateWall();
	UFUNCTION()
	void DestroyWallGeneratorActor();
	UFUNCTION()
	void SnapActor(float SnapValue);
	UFUNCTION()
	void EditWall();
	UFUNCTION()
	void EditDoor();
	UFUNCTION()
	void EditFloor();
	// Widget Bind Function - Wall
	UFUNCTION()
	void OnSegmentsChanged(float InValue);
	UFUNCTION()
	void OnWallBtnClicked();
	UFUNCTION()
	void SetWallStaticMesh(const FWallData& WallData);

	// Door Construction
	UPROPERTY()
	UStaticMesh* DoorMesh{};
	UPROPERTY()
	UInputMappingContext* DoorConstructionIMC;
	// Function - Door
	UFUNCTION()
	void SetupDoorConstructionInputs();
	UFUNCTION()
	void GenerateDoorOnClick();
	// Widget Bind Function - Door
	UFUNCTION()
	void OnDoorBtnClicked();
	UFUNCTION()
	void SetDoorMesh(const FDoorData& DoorData);
	
	// Floor Construction
	UPROPERTY()
	AFloorGenerator* FloorGeneratorActor;
	UPROPERTY(EditDefaultsOnly, Category = "ArchVizController | Floor")
	TSubclassOf<AFloorGenerator> FloorGeneratorActorRef;
	UPROPERTY()
	UInputMappingContext* FloorConstructionIMC;
	UPROPERTY()
	FVector StartFloorLocation{};
	UPROPERTY()
	bool bShouldEditFloorLocationUnderCursor;
	// Function - Floor
	UFUNCTION()
	void SetupFloorConstructionInputs();
	UFUNCTION()
	void PreviewFloor();
	UFUNCTION()
	void StartBuildingFloor();
	UFUNCTION()
	void CompleteBuildingFloor();
	// Widget Bind Function - Floor
	UFUNCTION()
	void OnFloorBtnClicked();
	UFUNCTION()
	void OnFloorLocationXValueChanged(float InValue);
	UFUNCTION()
	void OnFloorLocationYValueChanged(float InValue);
	UFUNCTION()
	void OnFloorDimensionXValueChanged(float InValue);
	UFUNCTION()
	void OnFloorDimensionYValueChanged(float InValue);
	UFUNCTION()
	void OnFloorDimensionZValueChanged(float InValue);
	UFUNCTION()
	void OnDestroyFloorBtnClicked();
	UFUNCTION()
	void OnUpdateFloorLocationUnderCursorBtnClicked();
	
	// Roof Construction
	UPROPERTY()
	ARoofGenerator* RoofGeneratorActor;
	UPROPERTY(EditDefaultsOnly, Category = "ArchVizController | Roof")
	TSubclassOf<ARoofGenerator> RoofGeneratorActorRef;
	UPROPERTY()
	UInputMappingContext* RoofConstructionIMC;
	// Function - Floor
	UFUNCTION()
	void SetupRoofConstructionInputs();
	UFUNCTION()
	void GenerateRoofOnClick();
	// Widget Bind Function - Roof
	UFUNCTION()
	void OnRoofBtnClicked();

};
