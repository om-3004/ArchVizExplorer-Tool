// Fill out your copyright notice in the Description page of Project Settings.


#include "TemplateActor.h"

ATemplateActor::ATemplateActor()
{
	PrimaryActorTick.bCanEverTick = true;

	TemplateSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TemplateSceneComponent"));
	RootComponent = TemplateSceneComponent;
}

void ATemplateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATemplateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

