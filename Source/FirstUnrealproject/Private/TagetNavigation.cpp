// Fill out your copyright notice in the Description page of Project Settings.


#include "TagetNavigation.h"
#include "Interfaces\MyVisitor.h"

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
	visitor.Visit(*this, Target->GetActorLocation());
}

void UTagetNavigation::GoToTarget(FVector target)
{

}


// Called when the game starts
void UTagetNavigation::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTagetNavigation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

