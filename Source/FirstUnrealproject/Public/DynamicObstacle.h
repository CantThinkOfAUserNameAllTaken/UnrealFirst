// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces\MyVisitable.h"
#include "NavGraph.h"
#include "DynamicObstacle.generated.h"
class IMyVisitor;
class ANavGraph;
class UMyDynamicObjectList;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UDynamicObstacle : public UActorComponent, public IMyVisitable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicObstacle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UMyDynamicObjectList* DA_DynamicObstacleCounter;

	virtual void BeginDestroy() override;

public:	
	void Accept(IMyVisitor& visitor) override;
	void UpdateObstaclePositionOnGrid(ANavGraph::GridSquare***& grid, FVector originalPos, float cellSize, int arrayZ, int arrayY, int arrayX);
	bool WithinArrayBounds(int ZPos, int arrayZ, int YPos, int arrayY, int XPos, int arrayX);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
