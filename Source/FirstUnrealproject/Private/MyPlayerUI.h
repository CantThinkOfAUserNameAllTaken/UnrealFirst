// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class UMyPlayerUI : public UUserWidget
{
	GENERATED_BODY()


public:
	int MaxHealth;

	UFUNCTION()
	void UpdateHealthBar(int CurrentHealth);

	void UpdateCurrentProgress();

	void UpdateHealthBarColor(float CurrentFillPercent);




protected:
	UPROPERTY(EditAnywhere)
	float Speed;

	FTimerHandle HealthBarChange;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	bool bCurrentlyDecreasing = false;

	float CurrentSpeed = Speed;

	float FillPercentFinal;
	
};
