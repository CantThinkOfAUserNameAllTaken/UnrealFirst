// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TagetNavigation.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UTagetNavigation : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTagetNavigation();

	FVector distanceFromTarget;

	void GoToTarget(FVector target);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
