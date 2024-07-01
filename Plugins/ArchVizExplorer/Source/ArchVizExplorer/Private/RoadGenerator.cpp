// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadGenerator.h"

// Sets default values
ARoadGenerator::ARoadGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoadProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh Component"));
	RootComponent = RoadProceduralMeshComponent;
}

// Called when the game starts or when spawned
void ARoadGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoadGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ARoadGenerator::AddNewVertex(const FVector& VertexCordinates) {
	return Vertices.Add(VertexCordinates);
}

int ARoadGenerator::AddUV(const FVector2D& UVCordinates) {
	return UVs.Add(UVCordinates);
}

void ARoadGenerator::DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2) {
	Triangles.Add(Vertex0);
	Triangles.Add(Vertex1);
	Triangles.Add(Vertex2);
}

void ARoadGenerator::GenerateRoad(const FVector& Dimensions)
{
	RoadMeasurements = Dimensions;
	FVector LocationOffset = {0, 0, Dimensions.Z / 2};
	
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Normals.Empty();
	UVs.Reset();
	Tangents.Reset();
	Colors.Reset();

	// Bottom Face
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 0 - - -
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 1 - + -
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 2 + - -
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 3 + + -

	// Top Face
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 4 - - +
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 5 - + +
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 + - +
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 + + +

	// Front Face
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 4 - - - 0
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 5 - + - 1
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 - - + 4
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 - + + 5

	// Back Face
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 8 + - - 2
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 9 + + - 3
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 10 + - + 6
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 11 + + + 7

	// Left Face
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 12 + - - 2
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 13 - - - 0
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 14 + - + 6
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 15 - - + 4

	// Right Face
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 16 + + - 3
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 17 - + - 1
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 18 + + + 7
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 19 - + + 5

	// Bottom Face Triangle
	DrawTriangleFromVertex(0, 1, 2);
	DrawTriangleFromVertex(1, 3, 2);

	// Top Face Triangle
	DrawTriangleFromVertex(4, 5, 6);
	DrawTriangleFromVertex(5, 7, 6);

	// Front Face Triangle
	DrawTriangleFromVertex(8, 9, 10);
	DrawTriangleFromVertex(9, 11, 10);

	// Back Face Triangle
	DrawTriangleFromVertex(12, 13, 14);
	DrawTriangleFromVertex(13, 15, 14);

	// Left Face Triangle
	DrawTriangleFromVertex(16, 17, 18);
	DrawTriangleFromVertex(17, 19, 18);

	// Right Face Triangle
	DrawTriangleFromVertex(20, 21, 22);
	DrawTriangleFromVertex(21, 23, 22);

	for (int32 i = 0; i < Vertices.Num(); i += 4) {
		AddUV(FVector2D{ 0.0, 1.0 });
		AddUV(FVector2D{ 1.0, 1.0 });
		AddUV(FVector2D{ 0.0, 0.0 });
		AddUV(FVector2D{ 1.0, 0.0 });
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

	RoadProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	if(RoadProceduralMeshMaterial) { 
		RoadMaterial = RoadProceduralMeshMaterial;
		RoadProceduralMeshComponent->SetMaterial(0, RoadMaterial);
	}
}

void ARoadGenerator::ApplyMaterialToRoadActor(UMaterialInterface* Material) {
	RoadMaterial = Material;

	UMaterialInstanceDynamic* DynamicRoadMaterial = UMaterialInstanceDynamic::Create(Material, this);
	if (DynamicRoadMaterial) {
		float TileX = (RoadMeasurements.X) / 200.0f;
		float TileY = 1;
		DynamicRoadMaterial->SetScalarParameterValue("TileX", TileX);
		DynamicRoadMaterial->SetScalarParameterValue("TileY", TileY);
		RoadProceduralMeshComponent->SetMaterial(0, DynamicRoadMaterial);
	}
	
}