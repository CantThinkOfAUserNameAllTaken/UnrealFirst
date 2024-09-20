// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RocketShip.generated.h"

UCLASS()
class ARocketShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARocketShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UnPossess();

	void MoveZ(float Input);

	void MoveY(float Input);

	UPROPERTY(EditAnywhere) 
	float speed;

	FTimerHandle movement;

	void Movement();

	float ZCounter;
	float YCounter;

	UPROPERTY(EditAnywhere)
	float Gravity;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ParentCube;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	float Accelaration;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
