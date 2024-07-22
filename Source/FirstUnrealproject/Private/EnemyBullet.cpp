// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"


void AEnemyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit)
{
	if (OtherActor->Tags.Contains(TagToHit)) {
		UPlayerHealth* playerHealth = OtherActor->FindComponentByClass<UPlayerHealth>();
		playerHealth->TakeDamage(DamageDealt);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("no tags"));
		for (const FName& tag : OtherActor->Tags) {
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *tag.ToString());
		}

	}
}
