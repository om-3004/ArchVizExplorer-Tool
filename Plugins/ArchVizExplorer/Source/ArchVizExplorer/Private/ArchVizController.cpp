// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"

AArchVizController::AArchVizController() : getLocation{ true }, isFirstClick{ true }, bIsInConstruction{ true }
{
	
}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (HomeWidgetClassRef) {
		HomeWidget = CreateWidget<UHomeWidget>(this, HomeWidgetClassRef);
		HomeWidget->AddToViewport();

		if (HomeWidget && HomeWidget->ModeSelectionDropdown) {
			HomeWidget->ModeSelectionDropdown->OnSelectionChanged.AddDynamic(this, &AArchVizController::OnModeSelectionChanged);
		}
	}
	if (RoadConstructionWidgetClassRef) {
		RoadConstructionWidget = CreateWidget<URoadConstructionWidget>(this, RoadConstructionWidgetClassRef);
		
		if (RoadConstructionWidget && RoadConstructionWidget->ModeToggleBtn) {
			RoadConstructionWidget->ModeToggleBtn->OnClicked.AddDynamic(this, &AArchVizController::OnModeToggleBtnClicked);
			RoadConstructionWidget->WidthValue->OnValueChanged.AddDynamic(this, &AArchVizController::OnWidthValueChanged);
			RoadConstructionWidget->LocationX->OnValueChanged.AddDynamic(this, &AArchVizController::OnLocationXValueChanged);
			RoadConstructionWidget->LocationY->OnValueChanged.AddDynamic(this, &AArchVizController::OnLocationYValueChanged);
			RoadConstructionWidget->NewRoadBtn->OnClicked.AddDynamic(this, &AArchVizController::GenerateNewRoad);
		}
	}
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetupRoadConstructionInputs();
}

void AArchVizController::SetupRoadConstructionInputs()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		RoadConstructionIMC = NewObject<UInputMappingContext>();

		UInputAction* ClickforRoadConstruction = NewObject<UInputAction>(this);
		ClickforRoadConstruction->ValueType = EInputActionValueType::Boolean;
		RoadConstructionIMC->MapKey(ClickforRoadConstruction, EKeys::LeftMouseButton);

		EnhancedInputComponent->BindAction(ClickforRoadConstruction, ETriggerEvent::Completed, this, &AArchVizController::GetRoadLocationOnClick);

	}
}

void AArchVizController::GetRoadLocationOnClick()
{
	if(bIsInConstruction){
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

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
	else {
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if (Cast<ARoadGenerator>(HitResult.GetActor()))
		{
			RoadGeneratorActor = Cast<ARoadGenerator>(HitResult.GetActor());
			RoadConstructionWidget->WidthValue->SetValue(RoadGeneratorActor->GetActorScale3D().Y * RoadDimensions.Y);
			RoadConstructionWidget->LocationX->SetValue(RoadGeneratorActor->GetActorLocation().X);
			RoadConstructionWidget->LocationY->SetValue(RoadGeneratorActor->GetActorLocation().Y);
		}
	}
}

void AArchVizController::GenerateNewRoad()
{
	isFirstClick = true;
	getLocation = true;
}

void AArchVizController::GenerateRoad()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector ActorSpawnLocation = (StartLocation + EndLocation) / 2;
	FRotator ActorSpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

	ARoadGenerator* SpawnedActor = GetWorld()->SpawnActor<ARoadGenerator>(ARoadGenerator::StaticClass(), ActorSpawnLocation, ActorSpawnRotation, SpawnParams);

	RoadDimensions.X = FVector::Dist(StartLocation, EndLocation);
	RoadDimensions.Y = 100;
	RoadDimensions.Z = 2;
	SpawnedActor->GenerateCube(FVector(RoadDimensions), FVector(0, 0, RoadDimensions.Z/2), Material);
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

void AArchVizController::OnModeSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectedItem == FString("View Mode")) {
		RoadConstructionWidget->RemoveFromParent();

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			Subsystem->ClearAllMappings();
		}
	}
	else if (SelectedItem == FString("Road Construction")) {
		if (RoadConstructionWidgetClassRef) {
			RoadConstructionWidget->AddToViewport();

			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(RoadConstructionIMC, 0);
			}
		}
	}
}

void AArchVizController::OnModeToggleBtnClicked()
{
	if (RoadConstructionWidget->ModeToggleBtnText->GetText().EqualTo(FText::FromString("Editor Mode"))) { // is in Construction Mode
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Construction Mode")); // switch to Editor Mode
		bIsInConstruction = false;

		RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Visible);
		RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Visible);
		RoadConstructionWidget->RoadConstructionMsg->SetVisibility(ESlateVisibility::Visible);
		RoadConstructionWidget->RoadConstructionMsg->SetText(FText::FromString("Select the Road which you want to Edit."));
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		RoadConstructionWidget->ModeToggleBtnText->SetText(FText::FromString("Editor Mode"));
		bIsInConstruction = true;

		RoadConstructionWidget->WidthBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->LocationBox->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->RoadConstructionMsg->SetVisibility(ESlateVisibility::Hidden);
		RoadConstructionWidget->NewRoadBtn->SetVisibility(ESlateVisibility::Visible);
	}
}

void AArchVizController::OnWidthValueChanged(float InValue)
{
	if(RoadGeneratorActor) {
		RoadGeneratorActor->SetActorScale3D(FVector(1, InValue / RoadDimensions.Y, 1));
	}
}

void AArchVizController::OnLocationXValueChanged(float InValue)
{
	if (RoadGeneratorActor) {
		RoadGeneratorActor->SetActorLocation(FVector(InValue, RoadGeneratorActor->GetActorLocation().Y, RoadGeneratorActor->GetActorLocation().Z));
	}
}

void AArchVizController::OnLocationYValueChanged(float InValue)
{
	if (RoadGeneratorActor) {
		RoadGeneratorActor->SetActorLocation(FVector(RoadGeneratorActor->GetActorLocation().X, InValue, RoadGeneratorActor->GetActorLocation().Z));
	}
}
