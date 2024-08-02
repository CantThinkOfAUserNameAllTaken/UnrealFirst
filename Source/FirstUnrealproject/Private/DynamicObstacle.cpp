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

void UDynamicObstacle::UpdateObstaclePositionOnGrid(MyGridSquare::GridSquare***& grid, FVector OriginalPos, float cellSize, int arrayZ, int arrayY, int arrayX)
{
	int ZPos, YPos, XPos;
	UpdatePositionOnGrid(OriginalPos, cellSize, ZPos, YPos, XPos);

	if (WithinArrayBounds(ZPos, arrayZ, YPos, arrayY, XPos, arrayX)) {

		DeregisterLastObstaclePosition();
		if (NotAlreadyObstacle(grid, ZPos, YPos, XPos)) {
			UE_LOG(LogTemp, Warning,
				TEXT("adding to function XPos: %d, YPos: %d, ZPos: %d, Xarray: %d"),
				XPos, YPos, ZPos, arrayX);
			grid[ZPos][YPos][XPos].SetContains(MyGridSquare::Obstacle);
			StoreCurrentPosition(grid, ZPos, YPos, XPos);
			return;
		}
		return;
	}
	UE_LOG(LogTemp, Warning,
		TEXT("OutOfBounds XPos: %d, YPos: %d, ZPos: %d, Xarray: %d"),
		XPos, YPos, ZPos, arrayX);
	return;


}





void UDynamicObstacle::UpdatePositionOnGrid(FVector OriginalPos, float cellSize, int& ZPos, int& YPos, int& XPos )
{
	AActor* Owner = GetOwner();
	FVector difference = GetOwner()->GetActorLocation() - OriginalPos;
	float ZOffset = GetOwner()->GetComponentsBoundingBox().GetSize().Z;
	ZPos = (difference.Z - ZOffset) / cellSize;
	YPos = difference.Y / cellSize;
	XPos = difference.X / cellSize;
}

bool UDynamicObstacle::WithinArrayBounds(int ZPos, int arrayZ, int YPos, int arrayY, int XPos, int arrayX)
{
	return ZPos < arrayZ && YPos < arrayY && XPos < arrayX && XPos >= 0 && YPos >= 0 && ZPos >= 0;
}



void UDynamicObstacle::DeregisterLastObstaclePosition()
{
	if (!LastPositon) {
		return;
	}
	LastPositon->SetContains(MyGridSquare::Empty);
	LastPositon = nullptr;
}

bool UDynamicObstacle::NotAlreadyObstacle(MyGridSquare::GridSquare***& grid, int ZPos, int YPos, int XPos)
{
	return !grid[ZPos][YPos][XPos].contains == MyGridSquare::Obstacle;
}

void UDynamicObstacle::StoreCurrentPosition(MyGridSquare::GridSquare***& grid, int ZPos, int YPos, int XPos)
{
	LastPositon = &grid[ZPos][YPos][XPos];
}



// Called every frame
void UDynamicObstacle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

