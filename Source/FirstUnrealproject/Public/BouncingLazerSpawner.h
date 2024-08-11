// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BouncingLazerSpawner.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API ABouncingLazerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABouncingLazerSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	float TimeBeforeShooting = 0;

	float TimeLastShot;

	UPROPERTY(EditAnywhere)
	int bounceTimes;

	void ShootLazer();

	float GetAngle(FHitResult& hit, FVector& end);

	float GetDotProduct(FVector& StartNormal, FVector EndNormal);

	const FVector Normalize(FVector Start);

	UPROPERTY(EditAnywhere)
	float LazerTravelDistance;

	UPROPERTY(EditAnywhere)
	FColor LazerColor;

	UPROPERTY(EditAnywhere)
	float LazerThickness;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
