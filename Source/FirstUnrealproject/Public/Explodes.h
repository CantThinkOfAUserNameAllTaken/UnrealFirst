// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Explodes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UExplodes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplodes();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereColider;

	UPROPERTY(EditAnywhere)
	FVector ExplosionRadius;

	UPROPERTY(EditAnywhere)
	float DetectionRadius;

	UPROPERTY(EditAnywhere)
	FName TagToHit;

	UPROPERTY(EditAnywhere)
	float ExplosionDelay;

	AActor* CollidedActor;

	bool Triggered = false;

	float TimeWhenTriggered;

	void Explode();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
