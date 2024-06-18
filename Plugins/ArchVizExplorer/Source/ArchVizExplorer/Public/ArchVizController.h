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

#include "ArchVizController.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	FVector RoadDimensions;

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	bool bIsInConstruction;

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	FHitResult HitResult;

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	FVector StartLocation;

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	FVector EndLocation;

	UPROPERTY(EditDefaultsOnly, Category = "MyPlayerController")
	ARoadGenerator* RoadGeneratorActor;

	UPROPERTY(EditDefaultsOnly, Category = "MyPlayerController")
	TSubclassOf<ARoadGenerator> RoadGeneratorActorRef;

	UPROPERTY(EditDefaultsOnly, Category = "MyPlayerController")
	UMaterialInterface* Material;

	bool isFirstClick;

	bool getLocation;

	UFUNCTION()
	void GetRoadLocationOnClick();

	UFUNCTION()
	void GenerateNewRoad();

	/*UFUNCTION()
	void SelectRoad();*/

	UFUNCTION()
	void OnModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnModeToggleBtnClicked();

	UFUNCTION()
	void OnWidthValueChanged(float InValue);

	UFUNCTION()
	void OnLocationXValueChanged(float InValue);

	UFUNCTION()
	void OnLocationYValueChanged(float InValue);

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	UInputMappingContext* RoadConstructionIMC;

protected:
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void SetupRoadConstructionInputs();

	UPROPERTY()
	UHomeWidget* HomeWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UHomeWidget> HomeWidgetClassRef;

	UPROPERTY()
	URoadConstructionWidget* RoadConstructionWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<URoadConstructionWidget> RoadConstructionWidgetClassRef;

public:
	AArchVizController();

	virtual void BeginPlay() override;

	UFUNCTION()
	void GenerateRoad();

	UFUNCTION()
	float FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2);
};
