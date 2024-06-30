// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorDesign.h"

AInteriorDesign::AInteriorDesign()
{
	PrimaryActorTick.bCanEverTick = true;

	InteriorSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("InteriorSceneComponent"));
	RootComponent = InteriorSceneComponent;
}


void AInteriorDesign::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteriorDesign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteriorDesign::SetInteriorStaticMesh(UStaticMesh* StaticMesh)
{
	if (InteriorStaticMeshComponent) {
		InteriorStaticMeshComponent->DestroyComponent();
		InteriorStaticMeshComponent = nullptr;
	}
	InteriorStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
	if (InteriorStaticMeshComponent) {
		InteriorStaticMeshComponent->SetupAttachment(InteriorSceneComponent);
		InteriorStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		if (StaticMesh) {
			InteriorStaticMesh = StaticMesh;
			InteriorStaticMeshComponent->SetStaticMesh(StaticMesh);
		}
	}
}