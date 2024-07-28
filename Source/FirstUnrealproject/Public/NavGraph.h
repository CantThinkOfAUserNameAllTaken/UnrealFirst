// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MyVisitor.h"
#include "NavGraph.generated.h"

using namespace std;
class UDynamicObstacle;
class UMyDyanmicObjectList;
class UTagetNavigation;
class UMyTargetNavigationList;

enum GridCanContain { Empty, Obstacle, Enemy, Player };

UCLASS()
class FIRSTUNREALPROJECT_API ANavGraph : public AActor, public IMyVisitor
{
	GENERATED_BODY()

public:


	struct GridSquare {
		FVector startPoint;
		FVector endPointX;
		FVector endPointY;
		float cellSize;
		FVector Center;
		GridCanContain contains;

		public :
			void SetContains(GridCanContain NowContains) {
			contains = NowContains;
			}

		GridSquare(FVector InStartPoint = FVector(0,0,0), float InCellSize = 0, GridCanContain NowContains = Empty)
			: startPoint(InStartPoint),
			endPointX(FVector(InStartPoint.X + InCellSize, InStartPoint.Y, InStartPoint.Z)),
			endPointY(FVector(InStartPoint.X, InStartPoint.Y + InCellSize, InStartPoint.Z)),
			cellSize(InCellSize),
			Center(FVector(startPoint.X + (cellSize / 2), startPoint.Y + (cellSize / 2), startPoint.Z + (cellSize / 2))),
		    contains(NowContains) {}

	};
	void Visit(UDynamicObstacle& dynamicObstacle) override;
	void Visit(UTagetNavigation& Navigator, FVector TargetLocation) override;
	void GetPositionOnGrid(AActor* MovingObject, float cellSize, int& ZPos, int& YPos, int& XPos);
	// Sets default values for this actor's proper
	ANavGraph();
	//static ANavGraph* instance;


	virtual void BeginDestroy() override;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UMyDynamicObjectList* DA_DynamicObstaclesList;

	UPROPERTY(EditAnywhere)
	UMyTargetNavigationList* DA_TargetNavigationList;

	float TimeLastShot = -888;

	UPROPERTY(EditANywhere)
	float UpdateDynamicObjectsPositionsInterval;
	FVector DefineSquareBounds(float baseX, float baseY, float baseZ);

	GridSquare*** Grid = nullptr;

	GridSquare*** CreateGrid();

	void AddRow(int height, int column);

	void AddColumn(int height);

	GridCanContain SquareContains(FVector boxCenter, float cellSize);

	void DoesCubeContainStaticActor(bool& bHit, const FVector& boxCenter, float cellSize, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypesToCheck, TArray<AActor*, FDefaultAllocator>& ActrosToIgnore, TArray<AActor*, FDefaultAllocator>& OverlappedActors);

	GridSquare CreateSquare(FVector& start, float cellSize);

	void DebugShowObstacle(FVector center);
	UPROPERTY(EditAnywhere)
	int NumRows;

	UPROPERTY(EditAnywhere)
	int NumColumns;

	UPROPERTY(EditAnywhere)
	int NumHeight;

	UPROPERTY(EditAnywhere)
	float tileSize;

	UPROPERTY(EditAnywhere)
	float LineThickness;

	UPROPERTY(EditAnywhere)
	float ObstacleSphereRadius;

	UPROPERTY(EditAnywhere) 
	float ObstacleSphereDetail;
	

	UPROPERTY(EditAnywhere)
	FColor GridColor;

	UPROPERTY(EditAnywhere)
	FColor ObstacleSphereColor;

	UPROPERTY(EditAnywhere)
	bool ShowGrid;

	UPROPERTY(EditAnywhere)
	int LayerPriority;

	UFUNCTION()
	void DrawDebugGrid(FVector startPosition, int rows, int columns, int gridNum, float cellSize, FColor color, int lifeTime);
	void DrawColumns(int totalColumns, FVector& startPosition, float cellSize, int gridNum, int totalRows, FColor& color, int totalGridNum, int lifeTime);
	void DrawYAxisZLines(int totalRows, FVector& start, float cellSize, FColor& color, int lifeTime);
	bool NotLastGrid(int gridNum, int totalGridNum);
	void DrawRows(int totalRows, FVector& startPosition, float cellSize, int gridNum, int totalColumns, FColor& color, int totalGridNum, int lifeTime);
	void DrawXAxisZLines(int totalColumns, FVector& start, float cellSize, FColor& color, int lifeTime);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChnagedEvent) override;

	float RoundTo1OrNegative1(float number);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
