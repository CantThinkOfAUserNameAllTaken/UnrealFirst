// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketShip.h"
#include "camera/cameraComponent.h"

// Sets default values
ARocketShip::ARocketShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera->SetupAttachment(RootComponent);
	ParentCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParentTurret"));
	ParentCube->SetupAttachment(RootComponent);
	ParentCube->SetStaticMesh(Mesh);

}

// Called when the game starts or when spawned
void ARocketShip::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(movement, this, &ARocketShip::Movement, 0.1, true, 0.1);
}

void ARocketShip::UnPossess()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->UnPossess();
}

void ARocketShip::MoveZ(float Input)
{
	if (Input <= 0) {
		Accelaration += 1.5f * GetWorld()->GetDeltaSeconds();
	}
	else {
		ZCounter += Input;
		Accelaration = 1;
	}

}

void ARocketShip::MoveY(float Input)
{	
	YCounter += Input;
    ParentCube->SetWorldRotation(FRotator(0, 0, YCounter));
}

void ARocketShip::Movement()
{
	FVector MoveBy = (ParentCube->GetUpVector() * Accelaration) * ZCounter;
	UE_LOG(LogTemp, Warning, TEXT("MoveBy : %0.5f"), ZCounter)
	FVector grav = FVector::UpVector * -Gravity;
	MoveBy.Z -= Gravity * Accelaration;
	SetActorLocation(GetActorLocation() + (MoveBy * speed));
	//YCounter = FMath::FInterpTo(YCounter, 0, GetWorld()->GetDeltaSeconds(), 5);
	ZCounter = FMath::FInterpTo(ZCounter, 0, GetWorld()->GetDeltaSeconds(), 5);

}

// Called every frame
void ARocketShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARocketShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("PossessTurret", IE_Pressed, this, &ARocketShip::UnPossess);
	PlayerInputComponent->BindAxis("X", this, &ARocketShip::MoveY);
	PlayerInputComponent->BindAxis("Z", this, &ARocketShip::MoveZ);

}

