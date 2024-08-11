// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncingLazerSpawner.h"
#include <cmath>

// Sets default values
ABouncingLazerSpawner::ABouncingLazerSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

}

// Called when the game starts or when spawned
void ABouncingLazerSpawner::BeginPlay()
{
	Super::BeginPlay();
	TimeLastShot = 0;
	
}

void ABouncingLazerSpawner::ShootLazer()
{
	int BounceLeft = bounceTimes;
	FVector Start = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FCollisionQueryParams collisonQuery;
	while (BounceLeft > 0) {
		FVector end = Forward * LazerTravelDistance;

		FHitResult hit;


		bool bhit = GetWorld()->LineTraceSingleByChannel(hit, Start, end, ECC_Visibility, collisonQuery);

		

		if (bhit) {
			DrawDebugLine(GetWorld(), Start, hit.ImpactPoint, LazerColor, false, TimeBeforeShooting, 3, LazerThickness);
			collisonQuery.ClearIgnoredActors();
			BounceLeft--;
			float angle = GetAngle(hit, end);
			FVector incident =  Start - hit.ImpactPoint;
			FVector ImpactNormal = FVector(hit.ImpactNormal.X, hit.ImpactNormal.Z, hit.ImpactNormal.Y);
			FVector bAlongA = ImpactNormal * GetDotProduct(ImpactNormal, incident);
			FVector Flipped = incident - (bAlongA + bAlongA) ;
			Forward = Normalize(Flipped);
			Start = hit.ImpactPoint;
			collisonQuery.AddIgnoredActor(hit.GetActor());
			UE_LOG(LogTemp, Warning, TEXT("Forward: %s  name: %s" ), *hit.ImpactNormal.ToCompactString(), *hit.GetActor()->GetName())
		}
		else {
			DrawDebugLine(GetWorld(), Start, end, LazerColor, false, TimeBeforeShooting, 3, LazerThickness);
			BounceLeft = 0;
		}
	}


}

float ABouncingLazerSpawner::GetAngle(FHitResult& hit, FVector& end)
{
	//FVector StartNormal = Normalize(hit.ImpactNormal);
	//FVector EndNormal = Normalize(end);
	//float DotProduct = GetDotProduct(StartNormal, EndNormal);
	return 0;//std::acos(DotProduct);
}

float ABouncingLazerSpawner::GetDotProduct(FVector& StartNormal, FVector EndNormal)
{
	return ((StartNormal.X * EndNormal.X) + (StartNormal.Z * EndNormal.Z) + (StartNormal.Y * EndNormal.Y));
}

const FVector ABouncingLazerSpawner::Normalize(FVector Start)
{
	float length = std::sqrt((Start.X* Start.X) + (Start.Z * Start.Z) + (Start.Y * Start.Y));
	return (Start / length);
}

// Called every frame
void ABouncingLazerSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetTimeSeconds() - TimeBeforeShooting > TimeLastShot) {
		TimeLastShot = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Shooting"));
		ShootLazer();
	}

}

