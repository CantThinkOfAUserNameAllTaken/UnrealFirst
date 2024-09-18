// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ObjectMacros.h"
#include "MyTurretDetection.generated.h"


UENUM(BlueprintType)
enum class DetectionTypes : uint8 {
	Wedge,
	Sphyrical,
	SphyricalSector
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMyTurretDetection : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyTurretDetection();
	UStaticMeshComponent* TurretCenter;

	UStaticMeshComponent* TopTurret;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CalculateWedgeDetectionRadius();

	void DrawDetectionRadius(FVector& TurretPosition, FVector& ForwardDirection, float angle, FVector& TopCenter);

	void CalculateNegativeBounds(FMatrix& rotationMatrix, FVector& ForwardDirection, FVector& TurretPosition);

	void CalculatePositiveBounds(FMatrix& rotationMatrix, FVector& ForwardDirection, FVector& TurretPosition);

	void RotateOnZ(FMatrix& rotationMatrix, float angle);

	void RotateOnX(FMatrix& rotationMatrix, float angle);

	UPROPERTY(EditAnywhere)
	float MaxDetectionRadius;

	UPROPERTY(EditAnywhere)
	float MinDetectionRadius;

	UPROPERTY(EditAnywhere)
	float DetectionWidth;



	FVector endPoint;

    UPROPERTY(EditAnywhere)
	AActor* Target;

	FVector PositiveTopOuterBounds;

	FVector NegativeTopOuterBounds;

	UPROPERTY(EditAnywhere)
	float DetectionHeight;

	FVector PositiveDirection;
	FVector NegativeDirection;

	FVector PositiveBottomOuterBounds;
	FVector NegativeBottomOuterBounds;

	FVector LastPosition = FVector::ZeroVector;

	bool IsWithinBorder(FVector OriginalDirection, FVector UpDirection, FVector TargetLocation, FVector (UMyTurretDetection::*CrossProduct)(FVector, FVector));

	FVector GetCrossProductLeft(FVector OriginalDirection, FVector UpDirection);

	FVector GetCrossProductRight(FVector OriginalDirection, FVector UpDirection);

	bool IsWithinCone();

	bool IsWithinDetectionRadiusXY(FVector TargetLocation);

	UPROPERTY(EditAnywhere)
	DetectionTypes DetectionMode = DetectionTypes::Wedge;

	bool IsWithinDetectionRadius(FVector TargetLocation);

	UPROPERTY(EditAnywhere)
	float AngularThresh;
	

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DrawSphereDetectionRadius();

		
};
