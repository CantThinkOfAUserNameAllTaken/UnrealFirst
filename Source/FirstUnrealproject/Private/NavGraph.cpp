// Fill out your copyright notice in the Description page of Project Settings.


#include "NavGraph.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DynamicObstacle.h"
#include <iostream>
#include <string>
#include "MyDynamicObjectList.h"

//ANavGraph* ANavGraph::instance = nullptr;

void ANavGraph::Visit(UDynamicObstacle& dynamicObstacle)
{
	dynamicObstacle.UpdateObstaclePositionOnGrid(Grid, GetActorLocation(), tileSize, NumHeight, NumColumns, NumRows);
}


// Sets default values
ANavGraph::ANavGraph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UProceduralMeshComponent* mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedual Mesh"));

}




// Called when the game starts or when spawned
void ANavGraph::BeginPlay()
{
	Super::BeginPlay();
	/*if (instance == nullptr) {
		instance = this;
	}
	else {
		Destroy();
	}*/
	DrawDebugGrid(GetActorLocation(), NumRows, NumColumns, NumHeight, tileSize, GridColor, -1);
	//TODO - make a decontructer for deallocating this memeory
	CreateGrid();
}



ANavGraph::GridSquare*** ANavGraph::CreateGrid()
{
	Grid = new GridSquare **[NumHeight];
	float baseZ = 0;
	for (int height = 0; height < NumHeight; height++) {
		float baseY = 0;
		AddColumn(height);
		for (int column = 0; column < NumColumns; column++) {
			float baseX = 0;
			AddRow(height, column);
			for (int row = 0; row < NumRows; row++) {
				FVector start = DefineSquareBounds(baseX, baseY, baseZ);
				Grid[height][column][row] = CreateSquare(start, tileSize);
				baseX += tileSize;
			}
			baseY += tileSize;
		}
		baseZ += tileSize;
	}
	return Grid;
}

void ANavGraph::AddColumn(int height)
{
	Grid[height] = new GridSquare * [NumColumns];
}

void ANavGraph::AddRow(int height, int column)
{
	Grid[height][column] = new GridSquare[NumRows];
}

FVector ANavGraph::DefineSquareBounds(float baseX, float baseY, float baseZ)
{
	FVector gridStart = GetActorLocation();
	FVector start = FVector(gridStart.X + baseX, gridStart.Y + baseY, gridStart.Z + baseZ);
	return start;
}

ANavGraph::GridSquare ANavGraph::CreateSquare(FVector& start, float cellSize)
{
	GridSquare square(start, cellSize, Empty);
	square.contains = SquareContains(square.Center, cellSize);
	return square;
}

GridCanContain ANavGraph::SquareContains(FVector boxCenter, float cellSize)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCheck;
	TArray<AActor*> ActrosToIgnore;
	TArray<AActor*> OverlappedActors;
	ObjectTypesToCheck.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	bool bHit;
	DoesCubeContainStaticActor(bHit, boxCenter, cellSize,
		ObjectTypesToCheck, ActrosToIgnore, OverlappedActors);
	if (!bHit) {
		return GridCanContain(Empty);
	}
	DebugShowObstacle(boxCenter);
	return GridCanContain(Obstacle);
}

void ANavGraph::DoesCubeContainStaticActor(bool& bHit,
	const FVector& boxCenter, float cellSize, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypesToCheck,
	TArray<AActor*, FDefaultAllocator>& ActrosToIgnore, TArray<AActor*, FDefaultAllocator>& OverlappedActors)
{
	FVector Extends = FVector(cellSize / 2, cellSize / 2, cellSize / 2);
	bHit = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), boxCenter,
		Extends, ObjectTypesToCheck, nullptr, ActrosToIgnore, OverlappedActors);
}



void ANavGraph::DebugShowObstacle(FVector center)
{
	DrawDebugSphere(GetWorld(), center, ObstacleSphereRadius, ObstacleSphereDetail,
		ObstacleSphereColor, ShowGrid, -1, LayerPriority, LineThickness);
}

void ANavGraph::DrawDebugGrid(FVector startPosition, int totalRows,
	int totalColumns, int totalGridNum , float cellSize, FColor color, int lifeTime)
{
	for (int height = 0; height <= totalGridNum; height++)
	{	
		DrawRows(totalRows, startPosition, cellSize, height, totalColumns, color, totalGridNum, lifeTime);
		DrawColumns(totalColumns, startPosition, cellSize, height, totalRows, color, totalGridNum, lifeTime);
	}

}

void ANavGraph::DrawRows(int totalRows, FVector& startPosition,
	float cellSize, int gridNum, int totalColumns, FColor& color, int totalGridNum, int lifeTime)
{
	for (int rowNum = 0; rowNum <= totalRows; rowNum++)
	{
		FVector start = startPosition + FVector(0, rowNum * cellSize, gridNum * cellSize);
		FVector end = start + FVector(totalColumns * cellSize, 0, 0);
		DrawDebugLine(GetWorld(), start, end, color, ShowGrid, lifeTime, LayerPriority, LineThickness);
		if (NotLastGrid(gridNum, totalGridNum)) {
			DrawXAxisZLines(totalColumns, start, cellSize, color, lifeTime);
		}
	}
}

bool ANavGraph::NotLastGrid(int gridNum, int totalGridNum)
{
	return gridNum != totalGridNum;
}

void ANavGraph::DrawXAxisZLines(int totalColumns, FVector& start, float cellSize, FColor& color, int lifeTime)
{
	for (int column = 0; column <= totalColumns; column++)
	{
		FVector startX = FVector(start.X + column * cellSize, start.Y, start.Z);
		FVector end = FVector(start.X + column * cellSize, start.Y, start.Z + cellSize);
		DrawDebugLine(GetWorld(), startX, end, color, ShowGrid, lifeTime, LayerPriority, LineThickness);
	}
}

void ANavGraph::DrawColumns(int totalColumns,
	FVector& startPosition, float cellSize, int gridNum, int totalRows,
	FColor& color, int totalGridNum, int lifeTime)
{
	for (int colNum = 0; colNum <= totalColumns; colNum++)
	{
		FVector start = startPosition + FVector(colNum * cellSize, 0, gridNum * cellSize);
		FVector end = start + FVector(0, totalRows * cellSize, 0);
		DrawDebugLine(GetWorld(), start, end, color, ShowGrid, lifeTime, LayerPriority, LineThickness);
		if (NotLastGrid(gridNum, totalGridNum)) {
			DrawYAxisZLines(totalRows, start, cellSize, color, lifeTime);
		}

	}
}

void ANavGraph::DrawYAxisZLines(int totalRows, FVector& start, float cellSize, FColor& color, int lifeTime)
{
	for (int rows = 0; rows <= totalRows; rows++)
	{
		FVector startY = FVector(start.X, start.Y + rows * cellSize, start.Z);
		FVector end = FVector(start.X, start.Y + rows * cellSize, start.Z + cellSize);
		DrawDebugLine(GetWorld(), startY, end, color, ShowGrid, lifeTime, LayerPriority, LineThickness);
	}
}







#if WITH_EDITOR
void ANavGraph::PostEditChangeProperty(FPropertyChangedEvent& PropertyChnagedEvent)
{
	Super::PostEditChangeProperty(PropertyChnagedEvent);
	if (ShowGrid) {
		DrawDebugGrid(GetActorLocation(), NumRows, NumColumns, NumHeight, tileSize, GridColor, -1);
	}
	else {
		FlushPersistentDebugLines(GetWorld());
	}
}
#endif

// Called every frame
void ANavGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetTimeSeconds() - UpdateDynamicObjectsPositionsInterval > TimeLastShot) {
		TimeLastShot = GetWorld()->GetTimeSeconds();
		TArray<UDynamicObstacle*> DynamicObstacleList = DA_DynamicObstaclesList->GetRegisteredObjects();
		for (int DynamicObject = 0; DynamicObject < DynamicObstacleList.Num(); DynamicObject++) {

			DynamicObstacleList[DynamicObject]->Accept(*this);
		}
	}
}

void ANavGraph::BeginDestroy()
{
	Super::BeginDestroy();
	if (Grid != nullptr) 
	{
		for (int height = 0; height < NumHeight; height++) 
		{
			for (int column = 0; column < NumColumns; column++) {
				delete[] Grid[height][column];
			}
			delete[] Grid[height];
		}
		delete Grid;
	}
	
}
