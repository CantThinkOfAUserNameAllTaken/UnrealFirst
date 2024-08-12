// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurretPlacer.generated.h"

class UMyTurretDetection;
UCLASS()
class FIRSTUNREALPROJECT_API ATurretPlacer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretPlacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UnPossessTurret();

	void MoveX(float Input);

	void MoveY(float Input);

	void AdjustRotationForSurface();

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ParentCube;

	UPROPERTY(EditAnywhere)
	UStaticMesh* mesh;

	UPROPERTY(EditAnywhere)
	UMyTurretDetection* detectionController;

};
