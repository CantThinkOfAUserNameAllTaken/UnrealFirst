// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TagetNavigation.h"
#include "MyBaseRunTimeSet.h"
#include "MyTargetNavigationList.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UMyTargetNavigationList : public UPrimaryDataAsset, public MyBaseRunTimeSet<UTagetNavigation>
{
	GENERATED_BODY()
	
};
