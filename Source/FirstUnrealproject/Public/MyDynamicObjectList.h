// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DynamicObstacle.h"
#include "MyBaseRunTimeSet.h"
#include "MyDynamicObjectList.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UMyDynamicObjectList : public UPrimaryDataAsset, public MyBaseRunTimeSet<UDynamicObstacle>
{
	GENERATED_BODY()
	
};
