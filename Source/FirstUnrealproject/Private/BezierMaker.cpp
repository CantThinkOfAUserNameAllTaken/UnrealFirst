// Fill out your copyright notice in the Description page of Project Settings.


#include "BezierMaker.h"

// Sets default values
ABezierMaker::ABezierMaker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABezierMaker::BeginPlay()
{
	Super::BeginPlay();
	LastBuildLocation = GetActorLocation();
    PlayerController = GetWorld()->GetFirstPlayerController();
}

void ABezierMaker::CreateBezierLine()
{

	FVector MousePositon, MouseDirection;
	PlayerController->DeprojectMousePositionToWorld(MousePositon, MouseDirection);
	FVector DirToMouse = (MousePositon - LastBuildLocation).GetSafeNormal();
	FVector MouseNormal = DirToMouse.Cross(GetActorForwardVector());
	MouseNormal *= 1;
	MouseNormal += MousePositon;
	LastBuildNormal *= 1;
	LastBuildNormal += MousePositon;
	float Time = 0;
	FVector LastPosition = LastBuildLocation;
	while (Time <= 1) {
		FVector A = FMath::Lerp(LastBuildLocation, LastBuildNormal, Time);
		FVector B = FMath::Lerp(LastBuildNormal, MouseNormal, Time);
		FVector C = FMath::Lerp(MouseNormal, MousePositon, Time);
		FVector D = FMath::Lerp(A, B, Time);
		FVector E = FMath::Lerp(B, C, Time);
		FVector Position = FMath::Lerp(D, E, Time);
		DrawDebugLine(GetWorld(), LastPosition, Position, FColor::Black, false, 1.0f, 3);
		LastPosition = Position;
		Time += 0.1f;
	}
	LastBuildLocation = MousePositon;
	LastBuildNormal = MouseNormal;

}

void ABezierMaker::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (PlayerController) {
		PlayerController->bShowMouseCursor = true;
		Possessed = true;
	}

}

// Called every frame
void ABezierMaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Possessed) {
		CreateBezierLine();
	}

}

// Called to bind functionality to input
void ABezierMaker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ABezierMaker::CreateBezierLine);
}

