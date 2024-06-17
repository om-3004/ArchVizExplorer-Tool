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

#include "ArchVizController.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController
{
	GENERATED_BODY()

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

protected:
	virtual void SetupInputComponent() override;

public:
	AArchVizController();

	virtual void BeginPlay() override;

	UFUNCTION()
	void GenerateRoad();

	UFUNCTION()
	float FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2);
};
