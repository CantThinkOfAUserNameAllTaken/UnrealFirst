// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces\MyVisitable.h"
#include "MyGridSquare.h"
#include "TagetNavigation.generated.h"
class IMyVisitor;
class UMyTargetNavigationList;
class UActorList;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UTagetNavigation : public UActorComponent, public IMyVisitable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTagetNavigation();

	FVector distanceFromTarget;

	void GoToTarget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

private:
	
	UPROPERTY(EditAnywhere)
	float Speed;
	AActor* Target;

	UPROPERTY(EditAnywhere)
	UMyTargetNavigationList* DA_TargetNavigationList;

	UPROPERTY(EditAnywhere)
	UActorList* DA_PlayerList;

	bool PathCompleted = false;

	TArray<MyGridSquare::GridSquare*> Path;

	int PathIndex = 0;

public:	
	// Called every frame
	void Accept(IMyVisitor& visitor) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
