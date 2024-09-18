// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshStatCalcualtor.h"

// Sets default values for this component's properties
UMeshStatCalcualtor::UMeshStatCalcualtor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeshStatCalcualtor::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	meshComponent = Owner->GetComponentByClass<UStaticMeshComponent>();
	mesh = meshComponent->GetStaticMesh();
	FStaticMeshLODResources& LOD = mesh->GetRenderData()->LODResources[0];
	const FPositionVertexBuffer* VertextBuffer = &LOD.VertexBuffers.PositionVertexBuffer;
	for (uint32 i = 0; i < VertextBuffer->GetNumVertices(); i++) {
		Vertecies.Add(VertextBuffer->VertexPosition(i));
	}
	int arrayNum = 0;
	const FRawStaticIndexBuffer* IndexBuffer = &LOD.IndexBuffer;
	for (int32 i = 0; i < IndexBuffer->GetNumIndices(); i += 3) {
		TArray<int> triangle;
		triangles.Add(triangle);
		for (int32 j = i; j < i + 3; j++) {
			if (j < IndexBuffer->GetNumIndices()) {
				triangles[arrayNum].Add(IndexBuffer->GetIndex(j));
			}

		}
		arrayNum++;
	}
	GetSurfaceArea();
	GetVolume();
	// ...
	
}

float UMeshStatCalcualtor::GetSurfaceArea()
{
	float SurfaceArea = 0;
	for (int triangle = 0; triangle < triangles.Num(); triangle++) {
		FVector3f A = Vertecies[triangles[triangle][0]];
		FVector3f B = Vertecies[triangles[triangle][1]];
		FVector3f C = Vertecies[triangles[triangle][2]];

		FVector3f Side1 = A - B;
		FVector3f Side2 = B - C;
		FVector3f Side3 = C - A;

		TArray<float> Lengths = SideLengths(Side1, Side2, Side3);
		float Area = (Lengths[0] * Lengths[1]) / 2;
		SurfaceArea += Area;

		
	}
	UE_LOG(LogTemp, Warning, TEXT("Surface Area: %0.8f"), SurfaceArea);
	return SurfaceArea;
}

TArray<float> UMeshStatCalcualtor::SideLengths(FVector3f Side1, FVector3f Side2, FVector3f Side3)
{
	TArray<float> Lengths;
	if (Side1.Length() < Side2.Length() || Side1.Length() < Side3.Length()) {
		Lengths.Add(Side1.Length());
	}
	if (Side2.Length() < Side3.Length() || Side2.Length() < Side1.Length()) {
		Lengths.Add(Side2.Length());
	}
	if (Lengths.Num() < 2) {
		Lengths.Add(Side3.Length());
	}
	return Lengths;
}

float UMeshStatCalcualtor::GetVolume()
{
	float Volume = 0;
	for (int triangle = 0; triangle < triangles.Num(); triangle++) {
		FVector3f A = Vertecies[triangles[triangle][0]];
		FVector3f B = Vertecies[triangles[triangle][1]];
		FVector3f C = Vertecies[triangles[triangle][2]];

		FVector3f Side1 = A - FVector3f::ZeroVector;
		FVector3f Side2 = B - FVector3f::ZeroVector;
		FVector3f Side3 = C - FVector3f::ZeroVector;


		float TetrahedronVolume = FVector3f::DotProduct(Side1, FVector3f::CrossProduct(Side2, Side3)) / 6.0f;
		Volume += FMath::Abs(TetrahedronVolume);


	}
	UE_LOG(LogTemp, Warning, TEXT("Volume: %0.8f"), Volume);
	return Volume;
}


// Called every frame
void UMeshStatCalcualtor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

