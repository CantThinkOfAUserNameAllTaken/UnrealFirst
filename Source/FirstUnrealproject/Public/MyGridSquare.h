// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FIRSTUNREALPROJECT_API MyGridSquare
{
public:
	MyGridSquare();
	~MyGridSquare();

	enum GridCanContain { Empty, Obstacle, Enemy, Player };

	struct GridSquare {
		FVector startPoint;
		FVector endPointX;
		FVector endPointY;
		float cellSize;
		FVector Center;
		GridCanContain contains;

	public:
		void SetContains(GridCanContain NowContains) {
			contains = NowContains;
		}

		GridSquare(FVector InStartPoint = FVector(0, 0, 0), float InCellSize = 0, GridCanContain NowContains = Empty)
			: startPoint(InStartPoint),
			endPointX(FVector(InStartPoint.X + InCellSize, InStartPoint.Y, InStartPoint.Z)),
			endPointY(FVector(InStartPoint.X, InStartPoint.Y + InCellSize, InStartPoint.Z)),
			cellSize(InCellSize),
			Center(FVector(startPoint.X + (cellSize / 2), startPoint.Y + (cellSize / 2), startPoint.Z + (cellSize / 2))),
			contains(NowContains) {}

	};

};
