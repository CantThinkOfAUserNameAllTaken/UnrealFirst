// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyTurretDetection.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMyTurretDetection : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyTurretDetection();
	UStaticMeshComponent* TurretCenter;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CalculateDetectionRadius();

	UPROPERTY(EditAnywhere)
	float DetectionRadius;

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

	FVector LastPosition;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
