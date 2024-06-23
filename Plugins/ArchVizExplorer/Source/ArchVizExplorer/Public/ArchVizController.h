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
	void BindWidgets();
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
	bool bFromEditorToConstruction;
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
	// Widget Bind Function - Floor
	UFUNCTION()
	void OnFloorBtnClicked();
	
	// Roof Construction
	// Widget Bind Function - Roof
	UFUNCTION()
	void OnRoofBtnClicked();

};
