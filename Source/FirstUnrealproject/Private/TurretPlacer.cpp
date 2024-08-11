// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPlacer.h"
#include "camera/cameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATurretPlacer::ATurretPlacer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USpringArmComponent* arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	arm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(arm);
	Camera->bUsePawnControlRotation = false;

	ParentCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParentTurret"));
	ParentCube->SetupAttachment(arm);
	ParentCube->SetStaticMesh(mesh);
}

// Called when the game starts or when spawned
void ATurretPlacer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretPlacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurretPlacer::UnPossessTurret()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->UnPossess();
}
void ATurretPlacer::MoveX(float Input)
{
	FVector XDirection = GetActorRightVector();
	AddMovementInput(XDirection, Input);
	AdjustRotationForSurface();
}
void ATurretPlacer::MoveY(float Input)
{
	FVector YDirection = GetActorForwardVector();
	AddMovementInput(YDirection, Input);
	AdjustRotationForSurface();
}
void ATurretPlacer::AdjustRotationForSurface()
{
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetForwardVector() * 10000;
	bool bhit = GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECC_Visibility, params);
	if (bhit) {
		FVector FacingDirection = (hit.ImpactPoint - Start).GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("hit normal: %s"), *hit.ImpactNormal.ToCompactString());
		FVector XDirection = FVector::CrossProduct( Camera->GetRightVector(), hit.Normal).GetSafeNormal();
		FVector YDirection = FVector::CrossProduct(hit.Normal, XDirection).GetSafeNormal();
		FMatrix RotationMatrix = FMatrix(XDirection, YDirection, hit.Normal, FVector::ZeroVector);
		FQuat rotation = FQuat(RotationMatrix);
		ParentCube->SetWorldLocation(hit.ImpactPoint);
		ParentCube->SetWorldRotation(rotation);

	}
}
// Called to bind functionality to input
void ATurretPlacer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("PossessTurret", IE_Pressed, this, &ATurretPlacer::UnPossessTurret);
	PlayerInputComponent->BindAxis("X", this, &ATurretPlacer::MoveX);
	PlayerInputComponent->BindAxis("Z", this, &ATurretPlacer::MoveY);
	

}



