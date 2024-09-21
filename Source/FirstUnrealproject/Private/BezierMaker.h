// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BezierMaker.generated.h"

UCLASS()
class ABezierMaker : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABezierMaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateBezierLine(bool Pernament);

	void UpdateLineNormals(float Input);

	FVector LastBuildLocation;
	FVector LastBuildNormal = FVector::UpVector;

	FVector MousePosition;
	FVector MouseNormal;
	APlayerController* PlayerController;

	virtual void PossessedBy(AController* NewController) override;

	bool Possessed = false;

	void DrawNewLine();

	float NormalMultiplyer = 0;

	class UProceduralMeshComponent* mesh;

	void CreateMesh(FVector LastPosition, FVector Position, float Time);

	FVector GetTangent(float Time);

	UPROPERTY(EditAnywhere)
	float BoxSize;

private:
	struct Square {
		TArray<FVector> Vertices;
		TArray<int> Triangles;
	    public :
			FVector TopLeft;
			FVector TopRight;
			FVector BottomLeft;
			FVector BottomRight;
	};
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
