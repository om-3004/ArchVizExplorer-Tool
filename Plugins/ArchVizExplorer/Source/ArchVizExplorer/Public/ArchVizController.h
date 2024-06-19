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
enum class EBuildingMode : uint8 {
	None,
	Wall,
	Door,
	Floor,
	Roof,
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

	// Home Widget
	UPROPERTY()
	UHomeWidget* HomeWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UHomeWidget> HomeWidgetClassRef;

	// Road Construction
	UFUNCTION()
	void SetupRoadConstructionInputs();
	UPROPERTY()
	URoadConstructionWidget* RoadConstructionWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<URoadConstructionWidget> RoadConstructionWidgetClassRef;

	// Building Construction Widget
	UPROPERTY()
	UBuildingConstructionWidget* BuildingConstructionWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UBuildingConstructionWidget> BuildingConstructionWidgetClassRef;
	
	// Wall Construction
	UFUNCTION()
	void SetupWallConstructionInputs();

private:
	UPROPERTY()
	EModeSelected CurrentSelectedMode;

	UPROPERTY()
	ERodeMode CurrentRoadMode;

	UPROPERTY()
	EBuildingMode CurrentBuildingMode;

	// Road Construction
	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	FVector RoadDimensions;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	bool isFirstClick;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	bool getLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	FHitResult HitResult;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	FVector StartLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	FVector EndLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	ARoadGenerator* RoadGeneratorActor;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	TSubclassOf<ARoadGenerator> RoadGeneratorActorRef;

	// Wall Construction
	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	AWallGenerator* WallGeneratorActor;

	UPROPERTY(EditDefaultsOnly, Category = "ArchVizController")
	TSubclassOf<AWallGenerator> WallGeneratorActorRef;

	UPROPERTY(EditDefaultsOnly, Category = "ArchVizController")
	UMaterialInterface* Material;

	UPROPERTY(VisibleDefaultsOnly, Category = "ArchVizController")
	FVector WallLocation;

	UFUNCTION()
	void GetRoadLocationOnClick();
	
	UFUNCTION()
	void BuildWallAtClick();

	UFUNCTION()
	void RotateWall();

	UFUNCTION()
	void GenerateNewRoad();

	UFUNCTION()
	void OnSegmentsChanged(float InValue);

	UFUNCTION()
	void OnWallBtnClicked();
	
	UFUNCTION()
	void OnDoorBtnClicked();
	
	UFUNCTION()
	void OnFloorBtnClicked();
	
	UFUNCTION()
	void OnRoofBtnClicked();

	UFUNCTION()
	void UpdateWidget();

	UFUNCTION()
	void UpdateBuildingMappings();

	UFUNCTION()
	void UpdateInputMappings();

	UFUNCTION()
	void SetDefaultMode();

	UFUNCTION()
	void OnModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void SnapActor(float SnapValue);

	UFUNCTION()
	void OnRoadModeToggleBtnClicked();

	UFUNCTION()
	void OnWidthValueChanged(float InValue);

	UFUNCTION()
	void OnLocationXValueChanged(float InValue);

	UFUNCTION()
	void OnLocationYValueChanged(float InValue);

	UFUNCTION()
	void DestroyWallGeneratorActor();

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	UInputMappingContext* RoadConstructionIMC;

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	UInputMappingContext* WallConstructionIMC;

	UFUNCTION()
	void GenerateRoad();

	UFUNCTION()
	void PreviewWall();

	UFUNCTION()
	void CreateWidgets();

	UFUNCTION()
	void BindWidgets();

	UFUNCTION()
	float FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2);
};
