// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealth.h"

// Sets default values for this component's properties
UBaseHealth::UBaseHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UBaseHealth::BeginPlay()
{
	Super::BeginPlay();
	_currentHealth = _maxHealth;
	// ...
	
}


// Called every frame
void UBaseHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseHealth::TakeDamage(int amount)
{
	_currentHealth -= amount;
	if (_currentHealth == 0) {
		OnDeath();
	}
	UE_LOG(LogTemp, Warning, TEXT("Taken Damage"));
}

void UBaseHealth::OnDeath()
{
	GetOwner()->Destroy();
}

