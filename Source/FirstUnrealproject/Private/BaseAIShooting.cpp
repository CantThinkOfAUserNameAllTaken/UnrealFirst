// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIShooting.h"

// Sets default values for this component's properties
UBaseAIShooting::UBaseAIShooting()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseAIShooting::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBaseAIShooting::ShootBullet()
{
	if (TargetFound() and CooldownOver()) {
		SpawnBullet();
	}
}

void UBaseAIShooting::SpawnBullet()
{
	AActor* owner = GetOwner();
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = owner;
	spawnParams.Instigator = owner->GetInstigator();
	APlayerBullet* bullet = GetWorld()->SpawnActor<APlayerBullet>(BP_Bullet,
		owner->GetActorLocation(), owner->GetActorRotation(), spawnParams);
	bullet->cameraFacingDirection = owner->GetActorForwardVector();
	bullet->Shoot();
}

bool UBaseAIShooting::TargetFound()
{
	if (DA_Target == NULL) {
		return false;
	}
	TArray<AActor*> targets = DA_Target->GetRegisteredObjects();
	if (targets.IsEmpty()) {
		return false;
	}
	else if (targets.Num() > 1) {
		return false;
	}
	return true;
	
}

bool UBaseAIShooting::CooldownOver()
{
	if (GetWorld()->GetTimeSeconds() - shootingInterval > _timeLastShot) {
		_timeLastShot = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}


// Called every frame
void UBaseAIShooting::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ShootBullet();
	// ...
}

