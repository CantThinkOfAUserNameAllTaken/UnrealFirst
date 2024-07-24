// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicObstacle.h"
#include "Interfaces\MyVisitor.h"
#include "cmath"
#include "iostream"
#include "MyDynamicObjectList.h"


// Sets default values for this component's properties
UDynamicObstacle::UDynamicObstacle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDynamicObstacle::BeginPlay()
{
	Super::BeginPlay();
	//ANavGraph* grid = ANavGraph::instance;
	// ...
	if (!DA_DynamicObstacleCounter) {
		return;
	}
	DA_DynamicObstacleCounter->RegisterObject(this);
}


void UDynamicObstacle::BeginDestroy()
{
	Super::BeginDestroy();
	if (!DA_DynamicObstacleCounter) {
		return;
	}
	DA_DynamicObstacleCounter->DeregisterObject(this);
}

void UDynamicObstacle::Accept(IMyVisitor& visitor)
{
	visitor.Visit(*this);
}

void UDynamicObstacle::UpdateObstaclePositionOnGrid(ANavGraph::GridSquare***& grid, FVector orginalPos, float cellSize, int arrayZ, int arrayY, int arrayX)
{
	FVector difference = GetOwner()->GetActorLocation() - orginalPos;
	float ZOffset = GetOwner()->GetComponentsBoundingBox().GetSize().Z;
	int ZPos = (difference.Z - ZOffset) / cellSize;
	int YPos = difference.Y / cellSize;
	int XPos = difference.X / cellSize;
	UE_LOG(LogTemp, Warning, TEXT("OutOfBounds difference Z: %.4f"), difference.Z - ZOffset);

	if (WithinArrayBounds(ZPos, arrayZ, YPos, arrayY, XPos, arrayX)) {

			if (!grid[ZPos][YPos][XPos].contains == Obstacle) {
				UE_LOG(LogTemp, Warning, TEXT("adding to function XPos: %d, YPos: %d, ZPos: %d, Xarray: %d"), XPos, YPos, ZPos, arrayX);
				grid[ZPos][YPos][XPos].SetContains(Obstacle);;
			}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OutOfBounds XPos: %d, YPos: %d, ZPos: %d, Xarray: %d"), XPos, YPos, ZPos, arrayX);
	}


}

bool UDynamicObstacle::WithinArrayBounds(int ZPos, int arrayZ, int YPos, int arrayY, int XPos, int arrayX)
{
	return ZPos < arrayZ && YPos < arrayY && XPos < arrayX && XPos >= 0 && YPos >= 0 && ZPos >= 0;
}

// Called every frame
void UDynamicObstacle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

