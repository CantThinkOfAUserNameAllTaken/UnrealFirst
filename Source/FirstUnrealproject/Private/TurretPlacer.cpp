// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPlacer.h"
#include "camera/cameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyTurretDetection.h"

// Sets default values
ATurretPlacer::ATurretPlacer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	arm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(arm);
	Camera->bUsePawnControlRotation = false;

	ParentCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParentTurret"));
	ParentCube->SetupAttachment(arm);
	ParentCube->SetStaticMesh(mesh);

	detectionController = CreateDefaultSubobject<UMyTurretDetection>(TEXT("Turret Detection"));
	detectionController->TurretCenter = ParentCube;

    TopTurret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopTurret"));
	TopTurret->SetupAttachment(ParentCube);
	TopTurret->SetStaticMesh(mesh);
	detectionController->TopTurret = TopTurret;
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
	AdjustRotationForSurface();

}

void ATurretPlacer::UnPossessTurret()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->UnPossess();
}
void ATurretPlacer::MoveX(float Input)
{
	if (Input == 0)
	{
		return;
	}
	FVector XDirection = GetActorRightVector();
	AddMovementInput(XDirection, Input);
	AdjustRotationForSurface();
}
void ATurretPlacer::MoveY(float Input)
{

	if (Input == 0)
	{
		return;
	}
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
	//DrawDebugLine(GetWorld(), Start, End, FColor::Magenta, false, 10, 5, 2);
	bool bhit = GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECC_Visibility, params);
	if (bhit) {
		FVector FacingDirection = (hit.ImpactPoint - Start).GetSafeNormal();
		//UE_LOG(LogTemp, Warning, TEXT("hit normal: %s"), *hit.ImpactNormal.ToCompactString());
		FVector XDirection = FVector::CrossProduct( Camera->GetRightVector(), hit.Normal).GetSafeNormal();
		FVector YDirection = FVector::CrossProduct(hit.Normal, XDirection).GetSafeNormal();
		FMatrix RotationMatrix = FMatrix(XDirection, YDirection, hit.Normal, FVector::ZeroVector);
		FQuat rotation = FQuat(RotationMatrix);
		FRotator XZrotator = rotation.Rotator();
		FRotator YAxisRotator = FRotator::ZeroRotator;
		YAxisRotator.Yaw += YawAdjustment;
		FQuat YAxis = YAxisRotator.Quaternion();

		//XZrotator.Yaw = 0;
		//rotation = XZrotator.Quaternion();
		//FQuat YAxis = rotator.Quaternion();
		//UE_LOG(LogTemp, Warning, TEXT("rotation: %s"), *rotation.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("YAxis: %s"), *YAxis.ToString());
		FQuat FinalRotaton = rotation * YAxis;

		//DrawDebugSphere(GetWorld(), hit.ImpactPoint, 10, 8, FColor::Magenta, false, 10, 3);
		ParentCube->SetWorldLocation(hit.ImpactPoint);
		ParentCube->SetWorldRotation(FinalRotaton);

	}
}
void ATurretPlacer::RotateTurret(float Input)
{
	if (Input == 0) {
		return;
	}
	YawAdjustment += Input;
	//FRotator rotation = ParentCube->GetComponentRotation();
	//rotation = rotation.Add(0, Input, 0);
	//ParentCube->SetWorldRotation(rotation);
	//AdjustRotationForSurface();
}
// Called to bind functionality to input
void ATurretPlacer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("PossessTurret", IE_Pressed, this, &ATurretPlacer::UnPossessTurret);
	PlayerInputComponent->BindAxis("X", this, &ATurretPlacer::MoveX);
	PlayerInputComponent->BindAxis("Z", this, &ATurretPlacer::MoveY);
	PlayerInputComponent->BindAxis("RotateTurret", this, &ATurretPlacer::RotateTurret);
	

}



