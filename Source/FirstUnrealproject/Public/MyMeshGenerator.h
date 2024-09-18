// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MyMeshGenerator.generated.h"

/**
 * 
 */

UCLASS()
class FIRSTUNREALPROJECT_API UMyMeshGenerator : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:

	void CreateMesh();



protected:
	FVector ClosestPoint(FVector point, TArray<FVector> Points);

	struct Vertex {
		Vertex* prev;
		Vertex* next;
		FVector point;
		float angle;
		Vertex(Vertex* PreviousVertex, Vertex* NextVertex, FVector point) :
			prev(PreviousVertex), next(NextVertex), point(point), angle(0.0f){}
	};

	TArray<Vertex*> ReflexPoints;

	float CalculateWedge(FVector Dir1, FVector Dir2);
	bool DoesPointContainTriangle(UMyMeshGenerator::Vertex* Last, FVector point);
	bool AreReflexPointsInTriangle(Vertex* Last);
	
};
