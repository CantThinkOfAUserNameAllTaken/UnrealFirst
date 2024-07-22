// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();

	FVector cameraFacingDirection;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* mesh;

	UFUNCTION()
	virtual void Shoot();

protected:

	UPROPERTY(EditAnywhere)
	float bulletForce = 5.0f;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	int DamageDealt;

	UPROPERTY(EditAnywhere)
	FName TagToHit;

};
