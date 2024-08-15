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
	CalculateDetectionRadius();
	// ...
	
}

void UMyTurretDetection::CalculateDetectionRadius()
{
	FVector TurretPosition = TurretCenter->GetComponentLocation();
	FVector ForwardDirection = TurretCenter->GetUpVector();
	endPoint = ForwardDirection * DetectionRadius;
	float angle = FMath::DegreesToRadians(DetectionWidth);
	FMatrix rotationMatrix = FMatrix
	(FPlane(1, 0, 0, 0),
		FPlane(0, FMath::Cos(angle), FMath::Sin(angle), 0),
		FPlane(0, -FMath::Sin(angle), FMath::Cos(angle), 0),
		FPlane(0, 0, 0, 1));
	PositiveDirection = rotationMatrix.TransformVector(ForwardDirection);
	PositiveTopOuterBounds = (PositiveDirection * DetectionRadius) + (TurretPosition + DetectionHeight/2);
	PositiveBottomOuterBounds = PositiveTopOuterBounds;
	PositiveBottomOuterBounds.Z -= DetectionHeight;
	DrawDebugSphere(GetWorld(), PositiveTopOuterBounds, 2, 8, FColor::Black, false, 10, 3);
	NegativeDirection = -rotationMatrix.TransformVector(ForwardDirection);
	NegativeTopOuterBounds = (NegativeDirection * DetectionRadius) + (TurretPosition + DetectionHeight/2);
	NegativeBottomOuterBounds = NegativeTopOuterBounds;
	NegativeBottomOuterBounds.Z -= DetectionHeight;
	DrawDebugSphere(GetWorld(), NegativeTopOuterBounds, 2, 8, FColor::Black, false, 10, 3);
	LastPosition = GetOwner()->GetActorLocation();
}


// Called every frame
void UMyTurretDetection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Target) {
		return;
	}
	if (LastPosition != GetOwner()->GetActorLocation()) {
		CalculateDetectionRadius();
	}
	FVector TargetDirection = (Target->GetActorLocation() - TurretCenter->GetComponentLocation()).GetSafeNormal();
	FVector PositiveDirectionLocal = PositiveTopOuterBounds.GetSafeNormal();
	FVector PerpendicularVectorLeft = FVector::CrossProduct(PositiveDirectionLocal, TargetDirection);
	float ToTheLeft = FVector::DotProduct(PerpendicularVectorLeft, TurretCenter->GetUpVector());
	FVector NegativeDirectionLocal = NegativeTopOuterBounds.GetSafeNormal();
	FVector PerpendicularVectorRight = FVector::CrossProduct(NegativeDirectionLocal, TargetDirection);
	float ToTheRight = FVector::DotProduct(PerpendicularVectorRight, TurretCenter->GetUpVector());
	float WithinZ = FMath::Abs(Target->GetActorLocation().Z - TurretCenter->GetComponentLocation().Z);
	if ((ToTheLeft < 0 && ToTheRight > 0 && FVector::DistSquared2D(Target->GetActorLocation(), TurretCenter->GetComponentLocation()) < DetectionRadius * DetectionRadius) && WithinZ < DetectionHeight) {
		UE_LOG(LogTemp, Warning, TEXT("Within detection whoo"));
	}
	// ...
}

