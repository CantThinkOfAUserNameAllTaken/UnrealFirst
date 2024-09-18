// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTurretDetection.h"

// Sets default values for this component's properties
UMyTurretDetection::UMyTurretDetection()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void UMyTurretDetection::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UMyTurretDetection::CalculateWedgeDetectionRadius()
{
	FVector TurretPosition = TurretCenter->GetComponentLocation();
	FVector ForwardDirection = TurretCenter->GetForwardVector();
	endPoint = ForwardDirection * MaxDetectionRadius;
	FVector TopCenter = FVector(TurretPosition.X, TurretPosition.Y, TurretPosition.Z + DetectionHeight);
	float angle = FMath::DegreesToRadians(DetectionWidth);
	FMatrix rotationMatrix;
	RotateOnZ(rotationMatrix, angle);
	CalculatePositiveBounds(rotationMatrix, ForwardDirection, TurretPosition);
	DrawDebugLine(GetWorld(), TurretPosition, PositiveTopOuterBounds, FColor::Black, false, 10, 3, 3);
	RotateOnZ(rotationMatrix, -angle);
	CalculateNegativeBounds(rotationMatrix, ForwardDirection, TurretPosition);
	DrawDetectionRadius(TurretPosition, ForwardDirection, angle, TopCenter);
	LastPosition = TurretCenter->GetComponentLocation();
}

void UMyTurretDetection::DrawDetectionRadius(FVector& TurretPosition, FVector& ForwardDirection, float angle, FVector& TopCenter)
{
	DrawDebugCircleArc(GetWorld(), TurretPosition, MaxDetectionRadius, ForwardDirection, angle, 1, FColor::Black, false, 100, 3, 3);
	DrawDebugCircleArc(GetWorld(), TopCenter, MaxDetectionRadius, ForwardDirection, angle, 1, FColor::Black, false, 100, 3, 3);
	DrawDebugCircleArc(GetWorld(), TurretPosition, MinDetectionRadius, ForwardDirection, angle, 1, FColor::Black, false, 100, 3, 3);
	DrawDebugCircleArc(GetWorld(), TopCenter, MinDetectionRadius, ForwardDirection, angle, 1, FColor::Black, false, 100, 3, 3);
	DrawDebugLine(GetWorld(), TopCenter, PositiveTopOuterBounds, FColor::Black, false, 100, 3, 3);
	DrawDebugLine(GetWorld(), TopCenter, NegativeTopOuterBounds, FColor::Black, false, 100, 3, 3);
}

void UMyTurretDetection::CalculateNegativeBounds(FMatrix& rotationMatrix, FVector& ForwardDirection, FVector& TurretPosition)
{
	NegativeDirection = rotationMatrix.TransformVector(ForwardDirection);
	NegativeTopOuterBounds = (NegativeDirection * MaxDetectionRadius) + (FVector(TurretPosition.X, TurretPosition.Y, TurretPosition.Z + (DetectionHeight)));;
	NegativeBottomOuterBounds = NegativeTopOuterBounds;
	DrawDebugLine(GetWorld(), TurretPosition, NegativeTopOuterBounds, FColor::Cyan, false, 10, 3, 3);
	NegativeBottomOuterBounds.Z -= DetectionHeight;
}

void UMyTurretDetection::CalculatePositiveBounds(FMatrix& rotationMatrix, FVector& ForwardDirection, FVector& TurretPosition)
{
	PositiveDirection = rotationMatrix.TransformVector(ForwardDirection);
	PositiveTopOuterBounds = (PositiveDirection * MaxDetectionRadius) + (FVector(TurretPosition.X, TurretPosition.Y, TurretPosition.Z + (DetectionHeight)));
	PositiveBottomOuterBounds = PositiveTopOuterBounds;
	PositiveBottomOuterBounds.Z -= DetectionHeight;
}

void UMyTurretDetection::RotateOnZ(FMatrix& rotationMatrix, float angle)
{
	rotationMatrix = FMatrix
	(

		FVector(FMath::Cos(angle), -FMath::Sin(angle), 0),
		FVector(FMath::Sin(angle), FMath::Cos(angle), 0),
		FVector(0, 0, 1),
		FVector(0, 0, 0));
}

void UMyTurretDetection::RotateOnX(FMatrix& rotationMatrix, float angle)
{
	rotationMatrix = FMatrix
	(
		FVector(1, 0, 0),
		FVector(0, FMath::Cos(angle), -FMath::Sin(angle)),
		FVector(0, FMath::Sin(angle), FMath::Cos(angle)),
		FVector(0, 0, 0));
}


bool UMyTurretDetection::IsWithinBorder(FVector OriginalDirection, FVector UpDirection, FVector TargetLocation, FVector (UMyTurretDetection::*CrossProduct)(FVector, FVector))
{
	FVector PerpendicularDirection = (this->*CrossProduct)(OriginalDirection, UpDirection);
	float DotForOriginal = FVector::DotProduct(OriginalDirection, TargetLocation);
	float DotForPerpendicular = FVector::DotProduct(PerpendicularDirection, TargetLocation);
	DrawDebugLine(GetWorld(), TurretCenter->GetComponentLocation(), (OriginalDirection * MaxDetectionRadius) + TurretCenter->GetComponentLocation(), FColor::Red, false, 1, 3, 3);
	DrawDebugLine(GetWorld(), TurretCenter->GetComponentLocation(), (PerpendicularDirection * 100) + TurretCenter->GetComponentLocation(), FColor::Blue, false, 1, 3, 3);
	bool bWithinRange = DotForOriginal >= 0 && DotForPerpendicular <= 0;
	//UE_LOG(LogTemp, Warning, TEXT("Original: %0.5f, Perpen: %0.5f, PerpenDir: %s"), DotForOriginal, DotForPerpendicular, *PerpendicularDirection.ToCompactString())
	return bWithinRange;
}

FVector UMyTurretDetection::GetCrossProductLeft(FVector OriginalDirection, FVector UpDirection)
{
	return FVector::CrossProduct(OriginalDirection, UpDirection);
}

FVector UMyTurretDetection::GetCrossProductRight(FVector OriginalDirection, FVector UpDirection)
{
	return FVector::CrossProduct(UpDirection, OriginalDirection);
}

bool UMyTurretDetection::IsWithinCone()
{
	FVector TargetLocation = Target->GetActorLocation();
	FVector UpDirection = TurretCenter->GetUpVector();
	FVector TurretToPlayer = TargetLocation - TurretCenter->GetComponentLocation(); 
	FVector TurretToPlayerLocal = UKismetMathLibrary::InverseTransformDirection(TurretCenter->GetComponentTransform(), TurretToPlayer);
	TurretToPlayer = TurretToPlayer.GetSafeNormal();
	
	bool bWithinLeft = IsWithinBorder(PositiveDirection.GetSafeNormal(), UpDirection, TurretToPlayer, &UMyTurretDetection::GetCrossProductLeft);
	bool bWithinRight = IsWithinBorder(NegativeDirection.GetSafeNormal(), UpDirection, TurretToPlayer, &UMyTurretDetection::GetCrossProductRight);
	bool bWithinHeight = TurretToPlayer.Z > NegativeBottomOuterBounds.Z && TurretToPlayer.Z < NegativeTopOuterBounds.Z;
	bool bWithinDistance = IsWithinDetectionRadiusXY(TargetLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Left: %d, Right: %d, Height: %d, Distance: %d"), bWithinLeft, bWithinRight, bWithinHeight, bWithinDistance);
	return (bWithinLeft && bWithinRight && bWithinHeight && bWithinDistance);
}

bool UMyTurretDetection::IsWithinDetectionRadiusXY(FVector TargetLocation)
{
	float distance = FVector::DistSquaredXY(TargetLocation, TurretCenter->GetComponentLocation());
	bool bWithinDistance = distance < MaxDetectionRadius * MaxDetectionRadius && distance > MinDetectionRadius * MinDetectionRadius;
	return bWithinDistance;
}
bool UMyTurretDetection::IsWithinDetectionRadius(FVector TargetLocation)
{
	float distance = FVector::DistSquared(TargetLocation, TurretCenter->GetComponentLocation());
	bool bWithinDistance = distance < MaxDetectionRadius * MaxDetectionRadius && distance > MinDetectionRadius * MinDetectionRadius;
	return bWithinDistance;
}

// Called every frame
void UMyTurretDetection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Target) {
		return;
	}
	if (DetectionMode == DetectionTypes::Wedge) {


		if (LastPosition != TurretCenter->GetComponentLocation()) {
			CalculateWedgeDetectionRadius();
		}

		if (IsWithinCone() && TopTurret) {
			FVector Direction = (Target->GetActorLocation() - TopTurret->GetComponentLocation()).GetSafeNormal();
			FRotator3d rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			rotation.Pitch = 0;
			UE_LOG(LogTemp, Warning, TEXT("rotation: %s"), *rotation.ToCompactString());
			TopTurret->SetRelativeRotation(rotation);

		}
	}

	if (DetectionMode == DetectionTypes::Sphyrical) {
		DrawSphereDetectionRadius();
		FVector TargetLocation = Target->GetActorLocation();
		bool bIsWithinDistance = IsWithinDetectionRadius(TargetLocation);
		if (bIsWithinDistance) {
			FVector Direction = (Target->GetActorLocation() - TopTurret->GetComponentLocation()).GetSafeNormal();
			FRotator3d rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			rotation.Pitch = 0;
			UE_LOG(LogTemp, Warning, TEXT("rotation: %s"), *rotation.ToCompactString());
			TopTurret->SetRelativeRotation(rotation);
		}

	}

	if (DetectionMode == DetectionTypes::SphyricalSector) {
		FVector TurretLocation = TurretCenter->GetComponentLocation();
		FVector Forward = TurretCenter->GetForwardVector();
		FVector Up = TurretCenter->GetUpVector();
		FVector Right = TurretCenter->GetRightVector();
		float y = FMath::Sqrt(1 - AngularThresh * AngularThresh);
		FVector TopPoint = (Forward * AngularThresh + Up * y) * MaxDetectionRadius;
		TopPoint += TurretLocation;
		FVector BottomPoint = (Forward * AngularThresh + Up * (-y)) * MaxDetectionRadius;
		BottomPoint += TurretLocation;
		FVector LeftPoint = (Forward * AngularThresh + Right * (-y)) * MaxDetectionRadius;
		FVector RightPoint = (Forward * AngularThresh + Right * y) * MaxDetectionRadius;
		
		LeftPoint += TurretLocation;
		RightPoint += TurretLocation;
		DrawDebugLine(GetWorld(),TurretLocation, TopPoint, FColor::Black, false, 1, 3, 3);
		DrawDebugLine(GetWorld(), TurretLocation, BottomPoint, FColor::Black, false, 1, 3, 3);
		DrawDebugLine(GetWorld(), TurretLocation, LeftPoint, FColor::Black, false, 1, 3, 3);
		DrawDebugLine(GetWorld(), TurretLocation, RightPoint, FColor::Black, false, 1, 3, 3);
		float c = FMath::Abs((LeftPoint - RightPoint).Length());
		float a = FMath::Abs((RightPoint - TurretLocation).Length());
		float b = FMath::Abs((LeftPoint - TurretLocation).Length());
		float A = FMath::Acos((b * b + c * c - a * a) / (2 * b * c));
		float B = FMath::Acos((c * c + a * a - b * b) / (2 * c * a));
		float C = (UKismetMathLibrary::GetTAU() / 2)-A-B;
		float angle = C / 2;
		UE_LOG(LogTemp, Warning, TEXT("angle: %0.5f"), angle);
		//UE_LOG(LogTemp, Warning, TEXT("a: %0.5f, c: %0.5f"), a, c);
		FVector CircleStartMax = FVector(TopPoint.X, TopPoint.Y, TurretLocation.Z);
		float MaxRadius = TopPoint.Z - TurretLocation.Z;
		FVector TopPointMin = (Forward * AngularThresh + Up * y) * MinDetectionRadius;
		TopPointMin += TurretLocation;
		FVector CircleStartMin = FVector(TopPointMin.X, TopPointMin.Y, TurretLocation.Z);
		float MinRadius = TopPointMin.Z - TurretLocation.Z;
		DrawDebugCircle(GetWorld(), CircleStartMax, MaxRadius, 32, FColor::Black, false, 100, 3, 3, Up, Right, false);
		DrawDebugCircleArc(GetWorld(), TurretLocation, MaxDetectionRadius, Forward, angle, 32, FColor::Black, false, 100, 3, 3);
		DrawDebugCircle(GetWorld(), CircleStartMin, MinRadius, 32, FColor::Black, false, 100, 3, 3, Up, Right, false);
		DrawDebugCircleArc(GetWorld(), TurretLocation, MinDetectionRadius, Forward, angle, 32, FColor::Black, false, 100, 3, 3);
		FVector DirToTargetWorld = Target->GetActorLocation() - TurretLocation;
		float Dot = FVector::DotProduct(DirToTargetWorld.GetSafeNormal(), Forward);
		if (Dot > AngularThresh && IsWithinDetectionRadius(Target->GetActorLocation())) {
			FVector Direction = (Target->GetActorLocation() - TopTurret->GetComponentLocation()).GetSafeNormal();
			FRotator3d rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			rotation.Pitch = 0;
			UE_LOG(LogTemp, Warning, TEXT("rotation: %s"), *rotation.ToCompactString());
			TopTurret->SetRelativeRotation(rotation);
		}
		

	}

	// ...
}

void UMyTurretDetection::DrawSphereDetectionRadius()
{
	DrawDebugSphere(GetWorld(), TurretCenter->GetComponentLocation(), MaxDetectionRadius, 8, FColor::Black, false, 100, 3, 3);
	DrawDebugSphere(GetWorld(), TurretCenter->GetComponentLocation(), MinDetectionRadius, 8, FColor::Black, false, 100, 3, 3);
}

