// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTurning.h"

// Sets default values for this component's properties
UBaseTurning::UBaseTurning()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseTurning::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}

void UBaseTurning::turnTowardsTarget(FVector position)
{
	AActor* Actor = GetOwner();
	FVector Direction = (position - Actor->GetActorLocation()).GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("Direction to player: %s"), *Direction.ToCompactString())
	FRotator3d TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	Actor->SetActorRotation(TargetRotation);
}

void UBaseTurning::Turing()
{
	if (DA_Counter == NULL) {
		return;
	}
	FVector targetPosition = GetPosition();
	turnTowardsTarget(targetPosition);
}

FVector UBaseTurning::GetPosition()
{
	TArray<AActor*> targets = DA_Counter->GetRegisteredObjects();
	if (targets.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("targets is empty!"));
		return FVector(0,0,0);
	}
	else if (targets.Num() > 1) {
		UE_LOG(LogTemp, Error, TEXT("targets has more than one element"));
		for (int i = 0; i < targets.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("name: %s"), *targets[i]->GetName());
		}
		return FVector(0, 0, 0);
	}
	return targets[0]->GetActorLocation();
}


// Called every frame
void UBaseTurning::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Turing();
	// ...
}

