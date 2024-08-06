// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MyVisitor.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "NavGraph.generated.h"

using namespace std;
class MyGridSquare;
class UDynamicObstacle;
class UMyDyanmicObjectList;
class UTagetNavigation;
class UMyTargetNavigationList;


UCLASS()
class FIRSTUNREALPROJECT_API ANavGraph : public AActor, public IMyVisitor
{
	GENERATED_BODY()

public:



	void Visit(UDynamicObstacle& dynamicObstacle) override;
	TArray<MyGridSquare::GridSquare*> Visit(UTagetNavigation& Navigator, AActor* Target) override;
	bool IsPathAtTarget(AActor* Target, AActor* MovingActor, int& ZPos, int& YPos, int& XPos,int YPath, int XPath);
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

	MyGridSquare::GridSquare*** Grid = nullptr;

	MyGridSquare::GridSquare*** CreateGrid();

	void AddRow(int height, int column);

	void AddColumn(int height);

	MyGridSquare::GridCanContain SquareContains(FVector boxCenter, float cellSize);

	void DoesCubeContainStaticActor(bool& bHit, const FVector& boxCenter, float cellSize, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypesToCheck, TArray<AActor*, FDefaultAllocator>& ActrosToIgnore, TArray<AActor*, FDefaultAllocator>& OverlappedActors);

	MyGridSquare::GridSquare CreateSquare(FVector& start, float cellSize);

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

	void DrawConnectingLine(FVector LineStart, FVector LineEnd);

	UPROPERTY(EditAnywhere)
	FColor LineColor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
