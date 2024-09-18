// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealth.h"
#include "MyPlayerUI.h"

void UPlayerHealth::BeginPlay()
{
	Super::BeginPlay();
	if (!PlayerUI) {
		return;
	}
	PlayerUI->AddToViewport();
	PlayerUI->MaxHealth = _maxHealth;
	OnHealthChanged.AddDynamic(PlayerUI, &UMyPlayerUI::UpdateHealthBar);
}



