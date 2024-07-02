// Fill out your copyright notice in the Description page of Project Settings.


#include "RoofGenerator.h"

ARoofGenerator::ARoofGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	RoofProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("RoofProceduralMeshComponent"));
	RootComponent = RoofProceduralMeshComponent;
}

void ARoofGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoofGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoofGenerator::GenerateRoof(const FVector& Dimensions)
{
	RoofMeasurements = Dimensions;
	GenerateCube(Dimensions, {0, 0, Dimensions.Z / 2});
}

void ARoofGenerator::GenerateCube(const FVector& Dimensions, const FVector& LocationOffset)
{
	if(RoofProceduralMeshMaterial) {RoofMaterial = RoofProceduralMeshMaterial;}

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

	RoofProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);

	ApplyMaterialToRoofActor(RoofMaterial);
}

void ARoofGenerator::ApplyMaterialToRoofActor(UMaterialInterface* Material)
{
	RoofMaterial = Material;
	RoofProceduralMeshMaterial = Material;

	UMaterialInstanceDynamic* DynamicRoofMaterial = UMaterialInstanceDynamic::Create(Material, this);
	if (DynamicRoofMaterial) {
		float TileX = (RoofMeasurements.X) / 200.0f;
		float TileY = (RoofMeasurements.Y) / 200.0f;
		DynamicRoofMaterial->SetScalarParameterValue("TileX", TileX);
		DynamicRoofMaterial->SetScalarParameterValue("TileY", TileY);
		RoofProceduralMeshComponent->SetMaterial(0, DynamicRoofMaterial);
	}
}
