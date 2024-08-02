// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces\MyVisitable.h"
#include "MyGridSquare.h"
#include "TagetNavigation.generated.h"
class IMyVisitor;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UTagetNavigation : public UActorComponent, public IMyVisitable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTagetNavigation();

	FVector distanceFromTarget;

	void GoToTarget(TArray<MyGridSquare::GridSquare*> Path);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditAnywhere)
	float Speed;
	
	UPROPERTY(EditAnywhere)
	AActor* Target;
public:	
	// Called every frame
	void Accept(IMyVisitor& visitor) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
