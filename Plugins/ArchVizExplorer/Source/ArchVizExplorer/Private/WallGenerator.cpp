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

	if (!WallActorMap.IsEmpty()) {
		for (auto& MapComponent : WallActorMap) {
			if(MapComponent.Value.ProceduralMeshComponent) {
				MapComponent.Value.ProceduralMeshComponent->DestroyComponent();
				MapComponent.Value.ProceduralMeshComponent = nullptr;
			}
		}
	}
}

void AWallGenerator::UpdateDoorsAndProceduralMeshComponent(int32 NoOfSegments) {
	if (!WallActorMap.IsEmpty() && !WallStaticMeshComponentsArr.IsEmpty()) {
		for (auto& MapComponent : WallActorMap) {
			if (MapComponent.Key <= (NoOfSegments - 1) && WallStaticMeshComponentsArr[MapComponent.Key]) {
				
				FVector MeshComponentLocation = WallStaticMeshComponentsArr[MapComponent.Key]->GetRelativeLocation();
				FVector WallDimensions = WallStaticMesh->GetBounds().GetBox().GetSize();
				MeshComponentLocation.Z -= WallDimensions.Z/2;

				WallStaticMeshComponentsArr[MapComponent.Key]->SetStaticMesh(MapComponent.Value.StaticMesh);
				WallStaticMeshComponentsArr[MapComponent.Key]->SetRelativeRotation(FRotator(0, 90, 0));
				WallStaticMeshComponentsArr[MapComponent.Key]->SetRelativeLocation(MeshComponentLocation);

				float DoorHeight = MapComponent.Value.StaticMesh->GetBounds().GetBox().GetSize().Z;
				FVector DoorLocation = WallStaticMeshComponentsArr[MapComponent.Key]->GetRelativeLocation();

				MapComponent.Value.ProceduralMeshComponent = NewObject<UProceduralMeshComponent>(this);
				FVector CubeLocation = FVector(DoorLocation.X, DoorLocation.Y, DoorLocation.Z + DoorHeight + ((WallDimensions.Z - DoorHeight)/2));
				FVector CubeDimensions = FVector(WallDimensions.X, WallDimensions.Y, WallDimensions.Z - DoorHeight);

				MapComponent.Value.ProceduralMeshComponent->SetupAttachment(SceneComponent);
				MapComponent.Value.ProceduralMeshComponent->RegisterComponentWithWorld(GetWorld());

				GenerateCube(CubeDimensions, FVector::ZeroVector, MapComponent.Value.ProceduralMeshComponent);
				MapComponent.Value.ProceduralMeshComponent->SetRelativeLocation(CubeLocation);
				MapComponent.Value.ProceduralMeshComponent->SetVisibility(true);

			}
		}
	}
}

void AWallGenerator::GenerateWall(const int32& NoOfSegments) {
	DestroyComponents();

	float LengthOfWallSegment{};
	if(WallStaticMesh) {
		LengthOfWallSegment = WallStaticMesh->GetBounds().GetBox().GetSize().X;
		HeightOfWall = WallStaticMesh->GetBounds().GetBox().GetSize().Z;
	}

	for(int i = 0 ; i < NoOfSegments ; i++) {
		WallStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		WallStaticMeshComponent->SetupAttachment(SceneComponent);
		WallStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		if (WallStaticMesh) {
			WallStaticMeshComponent->SetStaticMesh(WallStaticMesh);
		}
		WallStaticMeshComponent->SetRelativeLocation(FVector(i * LengthOfWallSegment, WallStaticMeshComponent->GetRelativeLocation().Y, HeightOfWall / 2));

		WallStaticMeshComponentsArr.Add(WallStaticMeshComponent);
	}

	UpdateDoorsAndProceduralMeshComponent(NoOfSegments);;
}

void AWallGenerator::GenerateCube(const FVector& Dimensions, const FVector& LocationOffset, UProceduralMeshComponent* CubeComponent)
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	// Bottom Face
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 0 - - -
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 1 - + -
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 2 + - -
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 3 + + -

	// Top Face
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 4 - - +
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 5 - + +
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 + - +
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 + + +

	// Front Face
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 4 - - - 0
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 5 - + - 1
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 - - + 4
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 - + + 5

	// Back Face
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 8 + - - 2
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 9 + + - 3
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 10 + - + 6
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 11 + + + 7

	// Left Face
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 12 + - - 2
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 13 - - - 0
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 14 + - + 6
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 15 - - + 4

	// Right Face
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 16 + + - 3
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 17 - + - 1
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 18 + + + 7
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 19 - + + 5

	// Bottom Face Triangle
	TArray<int32> Triangles{ 0, 1, 2 ,1, 3, 2 ,4, 5, 6 ,5, 7, 6 ,8, 9, 10,9, 11, 10 ,12, 13, 14 ,13, 15, 14 ,16, 17, 18 ,17, 19, 18 ,20, 21, 22 ,21, 23, 22 };

	for (int32 i = 0; i < Vertices.Num(); i += 4) {
		UVs.Add(FVector2D{ 0.0, 1.0 });
		UVs.Add(FVector2D{ 1.0, 1.0 });
		UVs.Add(FVector2D{ 0.0, 0.0 });
		UVs.Add(FVector2D{ 1.0, 0.0 });
	}

	for (int32 i = 0; i < Vertices.Num(); i++) {
		if (i >= 0 && i <= 3) {
			Normals.Add(FVector{ 0.0, 0.0, -1.0 });
		}
		else if (i >= 4 && i <= 7) {
			Normals.Add({ 0.0, 0.0, 1.0 });
		}
		else if (i >= 8 && i <= 11) {
			Normals.Add({ 0.0, -1.0, 0.0 });
		}
		else if (i >= 12 && i <= 15) {
			Normals.Add({ 0.0, 1.0, 0.0 });
		}
		else if (i >= 16 && i <= 19) {
			Normals.Add({ 1.0, 0.0, 0.0 });
		}
		else {
			Normals.Add({ -1.0, 0.0, 0.0 });
		}
	}

	CubeComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	CubeComponent->SetMaterial(0, ProceduralMeshMaterial);

}