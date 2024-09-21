// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BezierMaker.generated.h"

UCLASS()
class ABezierMaker : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABezierMaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateBezierLine();

	FVector LastBuildLocation;
	FVector LastBuildNormal = FVector::UpVector;

	APlayerController* PlayerController;

	virtual void PossessedBy(AController* NewController) override;

	bool Possessed = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
