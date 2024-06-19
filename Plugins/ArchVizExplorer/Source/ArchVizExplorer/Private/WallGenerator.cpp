// Fill out your copyright notice in the Description page of Project Settings.


#include "WallGenerator.h"

AWallGenerator::AWallGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComponent;
}

void AWallGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallGenerator::DestroyComponents()
{
	for (int i = 0; i < WallStaticMeshComponentsArr.Num(); i++) {
		if (WallStaticMeshComponentsArr[i]) {
			WallStaticMeshComponentsArr[i]->DestroyComponent();
			WallStaticMeshComponentsArr[i] = nullptr;
		}
	}
	WallStaticMeshComponentsArr.Empty();
}

void AWallGenerator::GenerateWall(const int32& NoOfSegments) {
	DestroyComponents();

	float LengthOfWallSegment = WallStaticMesh->GetBounds().GetBox().GetSize().X;

	for(int i = 0 ; i < NoOfSegments ; i++) {
		WallStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		WallStaticMeshComponent->SetupAttachment(SceneComponent);
		WallStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		if (WallStaticMesh) {
			WallStaticMeshComponent->SetStaticMesh(WallStaticMesh);
		}
		WallStaticMeshComponent->SetRelativeLocation(FVector(i * LengthOfWallSegment, WallStaticMeshComponent->GetRelativeLocation().Y, WallStaticMeshComponent->GetRelativeLocation().Z));

		WallStaticMeshComponentsArr.Add(WallStaticMeshComponent);
	}
}

