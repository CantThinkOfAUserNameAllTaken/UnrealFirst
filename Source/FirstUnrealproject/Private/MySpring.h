// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySpring.generated.h"

UCLASS()
class AMySpring : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySpring();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector AngleToDir(float RadiansAngle);

	FVector AngleToDirYZ(float RadiansAngle);

	UPROPERTY(EditAnywhere)
	float MinorRadius;

	UPROPERTY(EditAnywhere)
	int turns;

	UPROPERTY(EditAnywhere)
	int MajorRadius;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};