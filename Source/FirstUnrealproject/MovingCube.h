// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "PlayerBullet.h"
#include "ActorList.h"
#include "GameFramework/Character.h"
#include "MovingCube.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API AMovingCube : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMovingCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	void MoveOnX(float Input);

	void MoveOnZ(float Input);

	void RotateOnX(float Input);

	void RotateOnY(float Input);

	void Shoot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<APlayerBullet> BP_bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UActorList* DA_Counter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	float MaxYLookHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	float MinYLookHeight;

	APlayerController* playerController;
};
