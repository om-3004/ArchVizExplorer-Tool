// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"

AArchVizController::AArchVizController() : getLocation{ true }, isFirstClick{ true }
{
}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	bShowMouseCursor = true;
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>(this);

	UInputAction* ClickAction = NewObject<UInputAction>(this);
	ClickAction->ValueType = EInputActionValueType::Boolean;
	InputMappingContext->MapKey(ClickAction, EKeys::LeftMouseButton);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AArchVizController::GetRoadLocationOnClick);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AArchVizController::GetRoadLocationOnClick()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, RoadGeneratorActor);

	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	if (isFirstClick) {
		if (getLocation) {
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
				StartLocation = HitResult.Location;
			}
			getLocation = false;
		}
		else {
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
			{
				EndLocation = HitResult.Location;
			}
			GenerateRoad();
			isFirstClick = false;
		}
	}
	else {
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
		{
			FVector ForwardVectorOfPrevRoad = (EndLocation - StartLocation).GetSafeNormal();
			FVector LeftVectorOfPrevRoad = FVector::CrossProduct(ForwardVectorOfPrevRoad, FVector::UpVector);
			FVector BackVectorOfPrevRoad = (StartLocation - EndLocation).GetSafeNormal();
			FVector RightVectorOfPrevRoad = FVector::CrossProduct(FVector::UpVector, ForwardVectorOfPrevRoad);

			StartLocation = EndLocation;
			EndLocation = HitResult.Location;

			float Distance = FVector::Dist(EndLocation, StartLocation);

			//FVector LeftVectorOfCurrRoad = FVector::CrossProduct((EndLocation - StartLocation), FVector::UpVector);
			//FVector BackVectorOfCurrRoad = StartLocation - EndLocation;

			FVector ForwardVectorOfCurrRoad = EndLocation - StartLocation;
			float AngleOfPointWithLeftVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, LeftVectorOfPrevRoad);
			float AngleOfPointWithBackVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, BackVectorOfPrevRoad);

			FVector EndPointDir;
			if ((AngleOfPointWithLeftVecOfPrev >= 0 && AngleOfPointWithLeftVecOfPrev < 45) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev <= 90)) {
				EndPointDir = LeftVectorOfPrevRoad;
			}
			else if ((AngleOfPointWithLeftVecOfPrev > 135 && AngleOfPointWithLeftVecOfPrev <= 180) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev >= 90)) {
				EndPointDir = RightVectorOfPrevRoad;
			}
			else if (AngleOfPointWithLeftVecOfPrev > 45 && AngleOfPointWithLeftVecOfPrev <= 135) {
				EndPointDir = ForwardVectorOfPrevRoad;
			}
			EndLocation = StartLocation + (EndPointDir.GetSafeNormal() * Distance);
		}

		GenerateRoad();
	}
}

void AArchVizController::GenerateRoad()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ActorSpawnLocation = (StartLocation + EndLocation) / 2;
	FRotator ActorSpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

	ARoadGenerator* SpawnedActor = GetWorld()->SpawnActor<ARoadGenerator>(ARoadGenerator::StaticClass(), ActorSpawnLocation, ActorSpawnRotation, SpawnParams);

	FVector Dimensions = FVector(FVector::Dist(StartLocation, EndLocation));
	SpawnedActor->GenerateCube(FVector(Dimensions.X, 100, 2), Material);
}

float AArchVizController::FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2)
{
	FVector NormalizedVec1 = Vec1.GetSafeNormal();
	FVector NormalizedVec2 = Vec2.GetSafeNormal();

	float DotProduct = FVector::DotProduct(NormalizedVec1, NormalizedVec2);

	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);

	float AngleRadians = FMath::Acos(DotProduct);

	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	return AngleDegrees;
}