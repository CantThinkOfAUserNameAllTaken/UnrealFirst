// Fill out your copyright notice in the Description page of Project Settings.


#include "BezierMaker.h"
#include "ProceduralMeshComponent.h"

// Sets default values
ABezierMaker::ABezierMaker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));

}

// Called when the game starts or when spawned
void ABezierMaker::BeginPlay()
{
	Super::BeginPlay();
	LastBuildLocation = GetActorLocation();
    PlayerController = GetWorld()->GetFirstPlayerController();
	LastBuildNormal *= NormalMultiplyer;
	LastBuildNormal += LastBuildLocation;
}

void ABezierMaker::CreateBezierLine(bool Pernament)
{

	FVector MouseDirection;
	PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection);
	FHitResult hit;
	bool bhit = GetWorld()->LineTraceSingleByChannel(hit, MousePosition, MousePosition + (MouseDirection * 100000), ECC_Visibility);
	MousePosition = hit.Location;
	//MousePosition.X = LastBuildLocation.X;

	FVector DirToMouse = (MousePosition - LastBuildLocation).GetSafeNormal();
	MouseNormal = DirToMouse.Cross(GetActorForwardVector()).GetSafeNormal();
	MouseNormal *= NormalMultiplyer;
	MouseNormal += MousePosition;

	UE_LOG(LogTemp, Warning, TEXT("MousePosition: %s, LastBuildPosition: %s"), *MouseNormal.ToCompactString(), *LastBuildNormal.ToCompactString());
	float Time = 0;
	FVector LastPosition = LastBuildLocation;
	while (Time <= 1) {
		FVector A = FMath::Lerp(LastBuildLocation, LastBuildNormal, Time);
		FVector B = FMath::Lerp(LastBuildNormal, MouseNormal, Time);
		FVector C = FMath::Lerp(MouseNormal, MousePosition, Time);
		FVector D = FMath::Lerp(A, B, Time);
		FVector E = FMath::Lerp(B, C, Time);
		FVector Position = FMath::Lerp(D, E, Time);
		if (LastPosition != Position) {
			CreateMesh(LastPosition, Position, Time);
		}
		DrawDebugLine(GetWorld(), LastPosition, Position, FColor::Black, Pernament, 0.1f, 3, 3);
		LastPosition = Position;
		Time += 0.005f;
	}


}

void ABezierMaker::UpdateLineNormals(float Input)
{
	NormalMultiplyer += Input * 5;
}

void ABezierMaker::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (PlayerController) {
		PlayerController->bShowMouseCursor = true;
		Possessed = true;
	}

}

void ABezierMaker::DrawNewLine()
{
	CreateBezierLine(true);
	LastBuildLocation = MousePosition;
	LastBuildNormal = MouseNormal;
}

void ABezierMaker::CreateMesh(FVector LastPosition, FVector Position, float Time)
{
	FVector Tangent = GetTangent(Time).GetSafeNormal();
	FRotator NormalRotation = FRotator(0, 0, 90);
	FVector Normal = NormalRotation.RotateVector(Tangent);
	FVector Forward = Tangent.Cross(Normal);
	FVector LocalLast = GetTransform().InverseTransformPosition(LastPosition);
	FVector LocalPosition = GetTransform().InverseTransformPosition(Position);
	FVector LeftUp = (Normal + Forward).GetSafeNormal();
	FVector RightUp = (-Normal + Forward).GetSafeNormal();
	Square BottomFace;
	BottomFace.TopLeft = LocalLast + (Normal * BoxSize);
	BottomFace.TopRight = LocalLast + (-Normal * BoxSize);
	BottomFace.BottomLeft = LocalPosition + (Normal * BoxSize);
	BottomFace.BottomRight = LocalPosition + (-Normal * BoxSize);
	Square TopFace;
	TopFace.TopLeft = LocalLast + ((Normal * BoxSize) + (Forward * BoxSize));
	TopFace.TopRight = LocalLast + ((-Normal * BoxSize) + (Forward * BoxSize));
	TopFace.BottomLeft = LocalPosition + ((Normal * BoxSize) + (Forward * BoxSize));
	TopFace.BottomRight = LocalPosition + ((-Normal * BoxSize) + (Forward * BoxSize));
	Square LeftFace;
	LeftFace.TopLeft = BottomFace.TopLeft;
	LeftFace.BottomLeft = BottomFace.BottomLeft;
	LeftFace.TopRight = TopFace.TopLeft;
	LeftFace.BottomRight = TopFace.BottomLeft;
	Square RightFace;
	RightFace.TopLeft = BottomFace.TopRight;
	RightFace.BottomLeft = BottomFace.BottomRight;
	RightFace.TopRight = TopFace.TopRight;
	RightFace.BottomRight = TopFace.BottomRight;
	Square FrontFace;
	FrontFace.TopLeft = TopFace.BottomLeft;
	FrontFace.TopRight = TopFace.BottomRight;
	FrontFace.BottomLeft = BottomFace.BottomLeft;
	FrontFace.BottomRight = BottomFace.BottomRight;
	Square BackFace;
	BackFace.TopLeft = TopFace.TopLeft;
	BackFace.TopRight = TopFace.TopRight;
	BackFace.BottomLeft = BottomFace.TopLeft;
	BackFace.BottomRight = BottomFace.TopRight;
	//TODO -- Create Triangulate and Vertex function. pick a point then calculate triangles from it, 321, 302;
	//Create Mesh Section for each face, figure out what UVs, and tangents do;


}

FVector ABezierMaker::GetTangent(float Time)
{
	FVector A = LastBuildLocation * (-3 * (Time * Time) + (6 * Time) - 3);
	FVector B = LastBuildNormal * (9 * (Time * Time) - (12 * Time) + 3);
	FVector C = MouseNormal * (-9 * (Time * Time) + 6 * Time);
	FVector D = MousePosition * (3 * (Time * Time));
	return A + B + C + D;
}

// Called every frame
void ABezierMaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Possessed) {
		CreateBezierLine(false);
	}

}

// Called to bind functionality to input
void ABezierMaker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ABezierMaker::DrawNewLine);
	PlayerInputComponent->BindAxis("RotateTurret", this, &ABezierMaker::UpdateLineNormals);
}

