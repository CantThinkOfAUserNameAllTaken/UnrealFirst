// Fill out your copyright notice in the Description page of Project Settings.


#include "Explodes.h"

// Sets default values for this component's properties
UExplodes::UExplodes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UExplodes::BeginPlay()
{
	Super::BeginPlay();
	if (SphereColider) {

		SphereColider->SetCollisionProfileName(TEXT("Trigger"));
		SphereColider->SetWorldLocation(GetOwner()->GetActorLocation());
		SphereColider->InitSphereRadius(DetectionRadius);
		SphereColider->OnComponentBeginOverlap.AddDynamic(this, &UExplodes::OnOverlapBegin);
		SphereColider->SetWorldScale3D(FVector(DetectionRadius, DetectionRadius, DetectionRadius));
	}

	// ...
	
}


void UExplodes::Explode()
{
	FVector ExplosionPosition = GetOwner()->GetActorLocation();
	FVector ActorPositon = CollidedActor->GetActorLocation();
	FVector Distance = ActorPositon - ExplosionPosition;
	if (std::abs(Distance.X) < ExplosionRadius.X && std::abs(Distance.Y) <  ExplosionRadius.Y) {
		UE_LOG(LogTemp, Warning, TEXT("Within range"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("outside of range Disance: %s"), *Distance.ToString());
	}
	Triggered = false;
}

void UExplodes::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped %s"), *OtherActor->GetName());
	if (OtherActor->Tags.Contains(TagToHit) && !Triggered) {
		CollidedActor = OtherActor;
		Triggered = true;
		TimeWhenTriggered = GetWorld()->GetTimeSeconds();
	}
}


// Called every frame
void UExplodes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Triggered) {
		if (GetWorld()->GetTimeSeconds() - ExplosionDelay > TimeWhenTriggered) {
			Explode();
		}
	}
	// ...
}

