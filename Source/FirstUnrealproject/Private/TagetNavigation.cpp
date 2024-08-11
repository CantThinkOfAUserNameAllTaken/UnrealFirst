// Fill out your copyright notice in the Description page of Project Settings.


#include "TagetNavigation.h"
#include "Interfaces\MyVisitor.h"
#include "MyTargetNavigationList.h"
#include "ActorList.h"


// Sets default values for this component's properties
UTagetNavigation::UTagetNavigation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UTagetNavigation::Accept(IMyVisitor& visitor)
{
	if (!DA_PlayerList->GetRegisteredObjects().IsEmpty() && DA_PlayerList) {
		Target = DA_PlayerList->GetRegisteredObjects()[0];
		Path = visitor.Visit(*this, Target);
		if (!Path.IsEmpty()) {

			PathIndex = Path.Num() - 1;
			PathCompleted = true;
			return;
		}
		PathIndex = 0;
		PathCompleted = false;
	}

}

void UTagetNavigation::GoToTarget()
{
	AActor* MovingActor = GetOwner();
	FVector Objective = Path[PathIndex]->Center;
	MovingActor->SetActorLocation(FMath::VInterpConstantTo(MovingActor->GetActorLocation(), FVector(Objective.X, Objective.Y, MovingActor->GetActorLocation().Z), GetWorld()->GetDeltaSeconds(), Speed));
	if (MovingActor->GetActorLocation().X == Objective.X && MovingActor->GetActorLocation().Y == Objective.Y) {
		PathIndex--;
	}
	
	
}


// Called when the game starts
void UTagetNavigation::BeginPlay()
{
	Super::BeginPlay();

	if (!DA_TargetNavigationList || !DA_PlayerList) {
		return;
	}
	DA_TargetNavigationList->RegisterObject(this);
	// ...
	
}

void UTagetNavigation::BeginDestroy()
{
	Super::BeginDestroy();
	if (!DA_TargetNavigationList) {
		return;
	}
	DA_TargetNavigationList->DeregisterObject(this);
}


// Called every frame
void UTagetNavigation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PathCompleted && PathIndex > -1) {
		GoToTarget();
	}
	// ...
}

