// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BezierMaker.generated.h"
class UCameraComponent;

UCLASS()
class ABezierMaker : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABezierMaker();

	//UPROPERTY(EditAnywhere)
	//UCameraComponent* Camera;

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

	UPROPERTY(EditAnywhere)
	class UProceduralMeshComponent* mesh;


	void CreateMesh(FVector LastPosition, FVector Position, float Time, int MeshTempIndex);

	FVector GetTangent(float Time);

	UPROPERTY(EditAnywhere)
	float BoxSize;

private:
	struct Square {
		TArray<FVector> Vertices;
		TArray<int> Triangles;
		TArray<FColor> Color;

		TArray<FVector> Normal;
	    public :
			FVector TopLeft;
			FVector TopRight;
			FVector BottomLeft;
			FVector BottomRight;
			void Triangulate(FVector NormalDirection) {
				Vertices.Add(TopLeft);
				Vertices.Add(TopRight);
				Vertices.Add(BottomLeft);
				Vertices.Add(BottomRight);

				Triangles.Add(3);
				Triangles.Add(1);
				Triangles.Add(0);
				Triangles.Add(3);
				Triangles.Add(0);
				Triangles.Add(2);

				for (int i = 0; i < 4; i++) {
					Normal.Add(NormalDirection);
					Color.Add(FColor::Red);
				}
		}
	};
	int MeshIndex = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
