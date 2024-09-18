// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerUI.h"
#include "Components/ProgressBar.h"

void UMyPlayerUI::UpdateHealthBar(int CurrentHealth)
{
	FillPercentFinal = (float)CurrentHealth / MaxHealth;
	if (bCurrentlyDecreasing) {
		CurrentSpeed *= 2;
		return;
	}
	bCurrentlyDecreasing = true;
	UpdateCurrentProgress();
}

void UMyPlayerUI::UpdateCurrentProgress()
{
	if (!HealthBar) {
		return;
	}
	float CurrentFillPercent = HealthBar->GetPercent();
	CurrentFillPercent = FMath::FInterpTo(CurrentFillPercent, FillPercentFinal, GetWorld()->GetDeltaSeconds(), CurrentSpeed);
	
	HealthBar->SetPercent(CurrentFillPercent);
	UpdateHealthBarColor(CurrentFillPercent);
	if (CurrentFillPercent == FillPercentFinal) {
		CurrentSpeed = Speed;
		bCurrentlyDecreasing = false;
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(HealthBarChange, this, &UMyPlayerUI::UpdateCurrentProgress, 0.1, false, 0.1f);
}

void UMyPlayerUI::UpdateHealthBarColor(float CurrentFillPercent)
{
	if (CurrentFillPercent >= 0.75f) {
		HealthBar->SetFillColorAndOpacity(FLinearColor::Green);
		UE_LOG(LogTemp, Warning, TEXT("CurrentPercent: %0.5f"), CurrentFillPercent);
		return;
	}
	else if (CurrentFillPercent >= 0.25f) {
		HealthBar->SetFillColorAndOpacity(FLinearColor::Yellow);
		return;
	}
	else {
		HealthBar->SetFillColorAndOpacity(FLinearColor::Red);
		return;
	}
}


