// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseHealth.h"
#include "PlayerHealth.generated.h"

/**
 * 
 */
class UMyPlayerUI;
UCLASS()
class FIRSTUNREALPROJECT_API UPlayerHealth : public UBaseHealth
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMyPlayerUI* PlayerUI;


};
