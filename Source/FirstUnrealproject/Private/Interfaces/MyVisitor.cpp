// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/MyVisitor.h"
#include "DynamicObstacle.h"
#include "MyDynamicObjectList.h"
#include "TagetNavigation.h"


// Add default functionality here for any IMyVisitor functions that are not pure virtual.

void IMyVisitor::Visit(UDynamicObstacle& component)
{

}

TArray<MyGridSquare::GridSquare*> IMyVisitor::Visit(UTagetNavigation& component, AActor* actor)
{
	return TArray<MyGridSquare::GridSquare*>();
}

