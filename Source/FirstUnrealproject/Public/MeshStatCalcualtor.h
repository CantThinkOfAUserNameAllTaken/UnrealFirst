// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeshStatCalcualtor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UMeshStatCalcualtor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeshStatCalcualtor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UStaticMeshComponent* meshComponent;

	UStaticMesh* mesh;

	float GetSurfaceArea();

	TArray<FVector3f> Vertecies;
	TArray<TArray<int>> triangles;

	TArray<float> SideLengths(FVector3f Side1, FVector3f Side2, FVector3f Side3);

	float GetVolume();
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
