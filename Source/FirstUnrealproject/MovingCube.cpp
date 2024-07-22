// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingCube.h"
#include "camera/cameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBullet.h"
// Sets default values
AMovingCube::AMovingCube()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	camera->SetupAttachment(RootComponent);
	camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMovingCube::BeginPlay()
{
	Super::BeginPlay();
	DA_Counter->RegisterObject(camera->GetOwner());
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AMovingCube::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DA_Counter->DeregisterObject(camera->GetOwner());
	DA_Counter->GetRegisteredObjects().Empty();
}

// Called every frame
void AMovingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMovingCube::MoveOnX(float Input) {
	FVector XDirection = GetActorRightVector();
	AddMovementInput(XDirection, Input);
}

void AMovingCube::MoveOnZ(float Input) {
	FVector ZDirection = GetActorForwardVector();
	AddMovementInput(ZDirection, Input);
}
void AMovingCube::RotateOnX(float Input)
{
	AddControllerYawInput(Input);
}
void AMovingCube::RotateOnY(float Input)
{
	FRotator cameraRotation = playerController->GetControlRotation();
	if (cameraRotation.Pitch < MinYLookHeight || cameraRotation.Pitch > MaxYLookHeight) {
		return;
	}
	AddControllerPitchInput(Input);
}
void AMovingCube::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting bullet %s"), *BP_bullet->GetName());
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();
	APlayerBullet* bullet = GetWorld()->SpawnActor<APlayerBullet>(BP_bullet,
		GetActorLocation(), GetActorRotation(), spawnParams);
	bullet->cameraFacingDirection = camera->GetForwardVector();
	bullet->Shoot();
}
// Called to bind functionality to input
void AMovingCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAxis("X", this, &AMovingCube::MoveOnX);
	PlayerInputComponent->BindAxis("Z", this, &AMovingCube::MoveOnZ);
	PlayerInputComponent->BindAxis("TurnCameraX", this, &AMovingCube::RotateOnX);
	PlayerInputComponent->BindAxis("TurnCameraY", this, &AMovingCube::RotateOnY);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMovingCube::Shoot);
}

