// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "camera/cameraComponent.h"
#include "EnemyHealth.h"
#include "string"


// Sets default values
APlayerBullet::APlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	RootComponent = mesh;
	mesh->SetSimulatePhysics(true);
	mesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	mesh->SetNotifyRigidBodyCollision(true);
	mesh->OnComponentHit.AddDynamic(this, &APlayerBullet::OnHit);



}


// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerBullet::OnHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector normalImpulse, const FHitResult& Hit) {
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	if (OtherActor->Tags.Contains(TagToHit)) {
		UEnemyHealth* enemyHealth = OtherActor->FindComponentByClass<UEnemyHealth>();
		enemyHealth->TakeDamage(DamageDealt);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("no tags"));
		for (const FName& tag : OtherActor->Tags) {
			UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *tag.ToString());
		}

	}

	
}
void APlayerBullet::Shoot()
{
		mesh->AddImpulse(cameraFacingDirection * (bulletForce * 1000));
}

