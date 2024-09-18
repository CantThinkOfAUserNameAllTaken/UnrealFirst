// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpring.h"

// Sets default values
AMySpring::AMySpring()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySpring::BeginPlay()
{
	Super::BeginPlay();
	FVector origin = GetActorLocation();
	FVector NewOrigin = AngleToDir(FMath::DegreesToRadians(0));
	NewOrigin *= MajorRadius;
	NewOrigin += origin;
	FVector previousEnd = AngleToDir(FMath::DegreesToRadians(0));
	previousEnd *= MinorRadius;
	previousEnd += NewOrigin;
	FLinearColor colorStart = FLinearColor::MakeRandomColor();
	FLinearColor colorEnd = FLinearColor::MakeRandomColor();
	float InterpAmountPerTurn = 1.0f / (360.0f * turns);
	float InterpAmount = 0;
	double PercentPerRing = 360.0f / (turns * 360.0f);
	double totalPercentage = 0;
	while (colorStart == colorEnd) {
		colorStart = FLinearColor::MakeRandomColor();
	}
	for (int turn = 0; turn < turns; turn++) {
		for (int Degree = 0; Degree < 360; Degree++) {
			totalPercentage += PercentPerRing;
			double PercentToRadians = FMath::DegreesToRadians(totalPercentage);
			NewOrigin = AngleToDir(PercentToRadians);
			NewOrigin *= MajorRadius;
			NewOrigin += origin;
			float Radians = FMath::DegreesToRadians(Degree);
			FVector dir = AngleToDirYZ(Radians);
			dir = dir.RotateAngleAxis(totalPercentage, FVector::UpVector);
			dir *= MinorRadius;
			FVector end = NewOrigin + dir;
			FColor color = FLinearColor::LerpUsingHSV(colorStart, colorEnd, InterpAmount).ToFColor(true);
			DrawDebugLine(GetWorld(), previousEnd, end, color, true, -1, 3, 1);
			InterpAmount += InterpAmountPerTurn;
			previousEnd = end;

		}
	}
	
}

FVector AMySpring::AngleToDir(float RadiansAngle)
{
	return FVector(FMath::Cos(RadiansAngle), FMath::Sin(RadiansAngle), 0);
}

FVector AMySpring::AngleToDirYZ(float RadiansAngle)
{
	return FVector(FMath::Cos(RadiansAngle), 0, FMath::Sin(RadiansAngle));
}

// Called every frame
void AMySpring::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

