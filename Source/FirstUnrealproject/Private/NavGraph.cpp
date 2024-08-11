// Fill out your copyright notice in the Description page of Project Settings.


#include "NavGraph.h"
#include"MyGridSquare.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DynamicObstacle.h"
#include <iostream>
#include <string>
#include <queue>
#include "Containers/Array.h"
#include "MyDynamicObjectList.h"
#include "TagetNavigation.h"
#include "MyTargetNavigationList.h"

struct CompareNode {
	bool operator()(Node* const& n1, Node* const& n2) {
		if (n1->fCost == n2->fCost) {
			return n1->hCost > n2->hCost;
		}
		return n1->fCost > n2->fCost;
	}
};


void ANavGraph::Visit(UDynamicObstacle& dynamicObstacle)
{
	FVector ObstaclePosition = dynamicObstacle.UpdateObstaclePositionOnGrid(
		Grid,
		GetActorLocation(),
		tileSize,
		NumHeight,
		NumColumns,
		NumRows);
	DebugShowObstacle(ObstaclePosition, false, UpdateDynamicObjectsPositionsInterval);
}

TArray<MyGridSquare::GridSquare*> ANavGraph::Visit(UTagetNavigation& Navigator, AActor* Target)
{
	TArray<MyGridSquare::GridSquare*> Path;
	std::priority_queue<Node*, std::vector<Node*>, CompareNode> VisitedNodes;
	AActor* MovingActor = Navigator.GetOwner();
	FVector MovingActorLocation = MovingActor->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	int startZ, startY, startX;
	GetPositionOnGrid(MovingActor, tileSize, startZ, startY, startX);
	Node startNode(startX, startY);
	int endZ, endY, endX;
	GetPositionOnGrid(Target, tileSize, endZ, endY, endX);
	Node goalNode(endX, endY);
	startNode.hCost = CalculateHeuristic(startNode.x, startNode.y, goalNode.x, goalNode.y);
	VisitedNodes.push(&startNode);
	while (!VisitedNodes.empty()) {
		Node* CurrentNode = VisitedNodes.top();
		VisitedNodes.pop();
		if (VisitedNodes.size() > 10000) {
			return Path;
		}
		if (CurrentNode->hCost == goalNode.hCost) {
			while (CurrentNode != &startNode) {
				Path.Add(&Grid[0][CurrentNode->y][CurrentNode->x]);
				CurrentNode = CurrentNode->Parent;
				DrawConnectingLine(Path.Last()->Center, Grid[0][CurrentNode->y][CurrentNode->x].Center);
		    }
			//for (int i = 0; i < VisitedNodes.size(); i++) {
			//	delete VisitedNodes.top();
			//}
			return Path;
		}
		int directions[8][2] = { {1,0}, { 1,1 },{0,1}, { 1, -1 }, {-1, 1}, {-1,-1}, {-1, 0}, {0, -1} };
		for (auto& direction : directions) {
			Node* neibour = new Node(CurrentNode->x + direction[0], CurrentNode->y + direction[1]);
			if (!UDynamicObstacle::WithinArrayBounds(0, NumHeight, neibour->y, NumColumns, neibour->x, NumRows)
				|| Grid[0][neibour->y][neibour->x].contains == MyGridSquare::Obstacle) {
				continue;
			}
			neibour->Parent = CurrentNode;
			neibour->gCost = CurrentNode->gCost + direction[0] != 0 && direction[1] != 0 ? std::sqrt(2) : 1;
			neibour->hCost = CalculateHeuristic(neibour->x, neibour->y, goalNode.x, goalNode.y);
			neibour->fCost = neibour->gCost + neibour->hCost;
			VisitedNodes.push(neibour);
		}
	}

	return Path;

}

float ANavGraph::CalculateHeuristic(int x, int y, int endX, int endY) {
	int dx = std::abs(x - endX);
	int dy = std::abs(y - endY);
	return dx + dy + (std::sqrt(2) - 2) * std::min(dx, dy);
}



bool ANavGraph::IsPathAtTarget(AActor* Target, AActor* MovingActor, int& ZPos, int& YPos, int&XPos, int YPath, int XPath) {
	int MZPos, MYPos, MXPos;
	GetPositionOnGrid(MovingActor, tileSize, ZPos, YPos, XPos);
	GetPositionOnGrid(Target, tileSize, MZPos, MYPos, MXPos);
	YPos += YPath;
	XPos += XPath;
	return !(XPos == MXPos && YPos == MYPos);
}

void ANavGraph::GetPositionOnGrid(AActor* MovingObject, float cellSize, int& ZPos, int& YPos, int& XPos)
{
	FVector MovingObjectLocation = MovingObject->GetActorLocation();
	FVector difference = MovingObjectLocation - GetActorLocation();
	float ZOffset = MovingObject->GetComponentsBoundingBox().GetSize().Z;
	ZPos = (difference.Z - ZOffset) / cellSize;
	YPos = difference.Y / cellSize;
	XPos = difference.X / cellSize;

}

float ANavGraph::RoundTo1OrNegative1(float number)
{
	if (number > 0) {
		return 1;
	}
	else if (number < 0)
	{
		return -1;
	}
	return 0;
}

void ANavGraph::DrawConnectingLine(FVector LineStart, FVector LineEnd)
{
	DrawDebugLine(GetWorld(), LineStart, LineEnd, LineColor, false, UpdateDynamicObjectsPositionsInterval, LayerPriority, LineThickness);
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



MyGridSquare::GridSquare*** ANavGraph::CreateGrid()
{
	Grid = new MyGridSquare::GridSquare **[NumHeight];
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
	Grid[height] = new MyGridSquare::GridSquare * [NumColumns];
}

void ANavGraph::AddRow(int height, int column)
{
	Grid[height][column] = new MyGridSquare::GridSquare[NumRows];
}

FVector ANavGraph::DefineSquareBounds(float baseX, float baseY, float baseZ)
{
	FVector gridStart = GetActorLocation();
	FVector start = FVector(gridStart.X + baseX, gridStart.Y + baseY, gridStart.Z + baseZ);
	return start;
}

MyGridSquare::GridSquare ANavGraph::CreateSquare(FVector& start, float cellSize)
{
	MyGridSquare::GridSquare square(start, cellSize, MyGridSquare::Empty);
	square.contains = SquareContains(square.Center, cellSize);
	return square;
}

MyGridSquare::GridCanContain ANavGraph::SquareContains(FVector boxCenter, float cellSize)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCheck;
	TArray<AActor*> ActrosToIgnore;
	TArray<AActor*> OverlappedActors;
	ObjectTypesToCheck.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	bool bHit;
	DoesCubeContainStaticActor(bHit, boxCenter, cellSize,
		ObjectTypesToCheck, ActrosToIgnore, OverlappedActors);
	if (!bHit) {
		return MyGridSquare::GridCanContain(MyGridSquare::Empty);
	}
	DebugShowObstacle(boxCenter, true, -1);
	return MyGridSquare::GridCanContain(MyGridSquare::Obstacle);
}

void ANavGraph::DoesCubeContainStaticActor(bool& bHit,
	const FVector& boxCenter, float cellSize, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypesToCheck,
	TArray<AActor*, FDefaultAllocator>& ActrosToIgnore, TArray<AActor*, FDefaultAllocator>& OverlappedActors)
{
	FVector Extends = FVector(cellSize / 2, cellSize / 2, cellSize / 2);
	bHit = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), boxCenter,
		Extends, ObjectTypesToCheck, nullptr, ActrosToIgnore, OverlappedActors);
}



void ANavGraph::DebugShowObstacle(FVector center, bool Permanent, float LifeTime)
{
	DrawDebugSphere(GetWorld(), center, ObstacleSphereRadius, ObstacleSphereDetail,
		ObstacleSphereColor, Permanent, LifeTime, LayerPriority, LineThickness);
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
		TArray<UTagetNavigation*> TargetNavigationList = DA_TargetNavigationList->GetRegisteredObjects();
		for (int MovingActor = 0; MovingActor < TargetNavigationList.Num(); MovingActor++) {
			TargetNavigationList[MovingActor]->Accept(*this);
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
