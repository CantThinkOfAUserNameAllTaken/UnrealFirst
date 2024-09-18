// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyClock.generated.h"

class UMyMeshGenerator;

UENUM()
enum class ClockTickStates :uint8 {
	Instant,
	Gradual
};

UENUM()
enum class ClockTimeStates : uint8 {
	TwelveHour,
	TwentyFourHour
};

UCLASS()

class AMyClock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyClock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float ClockRadius;

	FVector LineEndMinute;
	FVector LineEndHour;
	FVector LineEndSecond;

	UWorld* World;

	FTimerHandle ClockSecondTimer;
	FTimerHandle ClockMinuteTimer;
	FTimerHandle ClockHourTimer;

	void UpdateSecondHand();

	void UpdateMinuteHand();

	void UpdateHourHand();

	FVector AngleToDir(float RadiansAngle);

	float SecondAngle = 80 + 10;

	float MinuteAngle = 80 + 10;

	float HourAngle = 80 + 10;
	

	
	UPROPERTY(EditAnywhere)
	ClockTickStates ClockTickState = ClockTickStates::Instant;

	UPROPERTY(EditAnywhere)
	ClockTimeStates ClockTime = ClockTimeStates::TwelveHour;

	UPROPERTY(EditAnywhere)
	UMyMeshGenerator* mesh;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
