// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\Fork-GitStuff\FirstUnrealproject\Source\FirstUnrealproject\PlayerBullet.h"
#include "PlayerHealth.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREALPROJECT_API AEnemyBullet : public APlayerBullet
{
	GENERATED_BODY()

protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit) override;
	
};
