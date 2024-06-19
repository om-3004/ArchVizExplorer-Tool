// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallGenerator.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AWallGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWallGenerator();

	UFUNCTION()
	void GenerateWall(const int32& NoOfSegments);

	UPROPERTY(EditDefaultsOnly, Category = "WallGenerator")
	UStaticMesh* WallStaticMesh;

	UFUNCTION()
	void DestroyComponents();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	UStaticMeshComponent* WallStaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "WallGenerator")
	TArray<UStaticMeshComponent*> WallStaticMeshComponentsArr;
};

