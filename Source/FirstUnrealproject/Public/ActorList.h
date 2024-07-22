// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "MyBaseRunTimeSet.h"
#include "Engine/DataAsset.h"
#include "ActorList.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API UActorList : public UPrimaryDataAsset, public MyBaseRunTimeSet<AActor>
{
	GENERATED_BODY()
	
};
