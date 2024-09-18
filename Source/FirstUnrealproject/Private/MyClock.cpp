// Fill out your copyright notice in the Description page of Project Settings.


#include "MyClock.h"
#include "MyMeshGenerator.h"

// Sets default values
AMyClock::AMyClock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UMyMeshGenerator>(TEXT("Mesh"));
	UMaterial* MyMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Materials/MyMaterial"));
	mesh->SetMaterial(0, MyMaterial);
}

// Called when the game starts or when spawned
void AMyClock::BeginPlay()
{
	Super::BeginPlay();
	mesh->CreateMesh();
	UpdateSecondHand();
	UpdateMinuteHand();
	UpdateHourHand();
	World = GetWorld();
	if (ClockTickState == ClockTickStates::Instant) {
		World->GetTimerManager().SetTimer(ClockSecondTimer, this, &AMyClock::UpdateSecondHand, 1, true, 1);
		World->GetTimerManager().SetTimer(ClockMinuteTimer, this, &AMyClock::UpdateMinuteHand, 60, true);
		World->GetTimerManager().SetTimer(ClockHourTimer, this, &AMyClock::UpdateHourHand, 3600, true);
	}
	else if (ClockTickState == ClockTickStates::Gradual) 
	{
		World->GetTimerManager().SetTimer(ClockSecondTimer, this, &AMyClock::UpdateSecondHand, 0.1f, true, 1);
		World->GetTimerManager().SetTimer(ClockMinuteTimer, this, &AMyClock::UpdateMinuteHand, 0.1f, true);
		World->GetTimerManager().SetTimer(ClockHourTimer, this, &AMyClock::UpdateHourHand, 0.1f, true);
	}
	

	for (int ClockTick = 0; ClockTick < 360; ClockTick += ClockTime == ClockTimeStates::TwelveHour ? 6 : 3) {
		float RadiansAngle = FMath::DegreesToRadians(ClockTick);
		FVector LineDir = AngleToDir(RadiansAngle);

		if (ClockTick % 5 == 0) {
			FVector LineStart = LineDir * (ClockRadius * 0.8 + 0.15);
			LineStart += GetActorLocation();
			FVector LineEnd = LineDir * ((ClockRadius * 0.8 + 0.15) * 1.4);
			LineEnd += GetActorLocation();
			DrawDebugLine(World, LineStart, LineEnd, FColor::Black, true, -1, 2, 5);
		}
		else {
			FVector LineStart = LineDir * (ClockRadius * 0.8 + 0.18);
			LineStart += GetActorLocation();
			FVector LineEnd = LineDir * ((ClockRadius * 0.80 + 0.18) * 1.4);
			LineEnd += GetActorLocation();
			DrawDebugLine(World, LineStart, LineEnd, FColor::Black, true, -1, 2, 3);
		}
	}
	DrawDebugCircle(GetWorld(), GetActorLocation(), ClockRadius, 32, FColor::Black, true, -1, 3, 3, GetActorUpVector(), GetActorRightVector(), false);
}

void AMyClock::UpdateSecondHand()
{
	float RadiansAngle = FMath::DegreesToRadians(SecondAngle);
	LineEndSecond = AngleToDir(RadiansAngle);
	LineEndSecond *= ClockRadius;
	LineEndSecond += GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Second called : %0.5f"), SecondAngle);
	DrawDebugLine(GetWorld(), GetActorLocation(), LineEndSecond, FColor::Red, false, ClockTickState == ClockTickStates::Gradual ? 0.1f : 1, 3, 3);
	if (ClockTickState == ClockTickStates::Gradual) {
		SecondAngle -= 0.6f;
		return;
	}
	SecondAngle -= 6;
}

void AMyClock::UpdateMinuteHand()
{
	float RadiansAngle = FMath::DegreesToRadians(MinuteAngle);
	LineEndMinute = AngleToDir(RadiansAngle);
	LineEndMinute *= ClockRadius;
	LineEndMinute += GetActorLocation();
	DrawDebugLine(GetWorld(), GetActorLocation(), LineEndMinute, FColor::Blue, false, ClockTickState == ClockTickStates::Gradual ? 0.1f : 60, 3, 3);
	if (ClockTickState == ClockTickStates::Gradual) {
		MinuteAngle -= 0.01f;
		return;
	}
	MinuteAngle -= 6;
}

void AMyClock::UpdateHourHand()
{
	float RadiansAngle = FMath::DegreesToRadians(HourAngle);
	LineEndHour = AngleToDir(RadiansAngle);
	LineEndHour *= ClockRadius;
	LineEndHour += GetActorLocation();
	DrawDebugLine(GetWorld(), GetActorLocation(), LineEndHour, FColor::Black, false, ClockTickState == ClockTickStates::Gradual ? 0.1f : 3600, 3, 3);
	if (ClockTickState == ClockTickStates::Gradual) {
		//UE_LOG(LogTemp, Warning, TEXT("hOUR called : %0.5f"), HourAngle);
		HourAngle -= ClockTime == ClockTimeStates::TwelveHour ? 0.000166667f : 0.0000833335;
		return;
	}
	HourAngle -= ClockTime == ClockTimeStates::TwelveHour ? 6 : 3;
}

FVector AMyClock::AngleToDir(float RadiansAngle)
{
	return FVector(0, FMath::Cos(RadiansAngle), FMath::Sin(RadiansAngle));
}

// Called every frame
void AMyClock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

