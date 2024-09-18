// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMeshGenerator.h"

void UMyMeshGenerator::CreateMesh()
{
#pragma region old



	//TArray<FVector> TopVertices;

	//TopVertices.Add(FVector(0, -10.0f, 5.0f));
	//TopVertices.Add(FVector(0, 0, 4.0f));
	//TopVertices.Add(FVector(0, 2.0f, 8.0f));
	//TopVertices.Add(FVector(0, 2, 12.0f));




	//TArray<FVector> BottomVertices;
	//BottomVertices.Add(FVector(0, -12.0f, -5.0f));
	//BottomVertices.Add(FVector(0, -2.0f, -8.0f));
	//BottomVertices.Add(FVector(0, 3.0f, -5.0f));
	//BottomVertices.Add(FVector(0, 5, 5));
	//BottomVertices.Add(FVector(0, 5, 12));

	//TArray<FVector> Verticies;

	//TArray<int32> Triangles;

	//TArray<FVector2D> UV;

	//TArray<FVector> Normals;

	//TArray<FProcMeshTangent> Tangents;

	//for (int Top = 0; Top < TopVertices.Num(); Top++) {
	//	TArray<FVector> Points;
	//	Points.Add(TopVertices[Top]);
	//	Points.Add(ClosestPoint(TopVertices[Top], TopVertices));
	//	Points.Add(ClosestPoint(TopVertices[Top], BottomVertices));

	//	DrawDebugLine(GetWorld(), Points[0], Points[1], FColor::Black, true, -1, 3);

	//	DrawDebugLine(GetWorld(), Points[1], Points[2], FColor::Black, true, -1, 3);

	//	DrawDebugLine(GetWorld(), Points[2], Points[0], FColor::Black, true, -1, 3);
	//	for (int Point = 0; Point < Points.Num(); Point++) {
	//		if (!Verticies.Contains(Points[Point])) {
	//			Verticies.Add(Points[Point]);
	//			UV.Add(FVector2D::ZeroVector);
	//			Normals.Add(FVector::UpVector);
	//			Tangents.Add(FProcMeshTangent::FProcMeshTangent());
	//			Triangles.Add(Verticies.Num() - 1);
	//		}
	//		else {
	//			Triangles.Add(Verticies.Find(Points[Point]));
	//		}
	//	}
	//}

	//for (int Bottom = 0; Bottom < BottomVertices.Num(); Bottom++) {
	//	TArray<FVector> Points;
	//	Points.Add(BottomVertices[Bottom]);
	//	Points.Add(ClosestPoint(BottomVertices[Bottom], TopVertices));
	//	Points.Add(ClosestPoint(BottomVertices[Bottom], BottomVertices));
	//	DrawDebugLine(GetWorld(), Points[0], Points[1], FColor::Black, true, -1, 3);

	//	DrawDebugLine(GetWorld(), Points[1], Points[2], FColor::Black, true, -1, 3);

	//	DrawDebugLine(GetWorld(), Points[2], Points[0], FColor::Black, true, -1, 3);
	//	for (int Point = 0; Point < Points.Num(); Point++) {
	//		if (!Verticies.Contains(Points[Point])) {
	//			Verticies.Add(Points[Point]);
	//			UV.Add(FVector2D::ZeroVector);
	//			Normals.Add(FVector::UpVector);
	//			Tangents.Add(FProcMeshTangent::FProcMeshTangent());
	//			Triangles.Add(Verticies.Num() - 1);
	//		}
	//		else {
	//			Triangles.Add(Verticies.Find(Points[Point]));
	//		}
	//	}
	//}
	//
	//CreateMeshSection_LinearColor(0, Verticies, Triangles, Normals, UV, TArray<FLinearColor>(), Tangents, false, false);
#pragma endregion
	Vertex start = Vertex(nullptr, nullptr, FVector(0, -10.0f, 5.0f));
	TArray<FVector> TopVertices;
	TopVertices.Add(FVector(0, -10.0f, 5.0f));
	TopVertices.Add(FVector(0, -7, 2.0f));
	TopVertices.Add(FVector(0, 0.0f, 7.0f));
	TopVertices.Add(FVector(0, 0, 12.0f));
	TopVertices.Add(FVector(0, 5, 12));
	TopVertices.Add(FVector(0, 5, 5));
	TopVertices.Add(FVector(0, 3.0f, -5.0f));
	TopVertices.Add(FVector(0, -2.0f, -8.0f));
	TopVertices.Add(FVector(0, -12.0f, -5.0f));
	Vertex* Last = &start;
	for (int point = 1; point < TopVertices.Num(); point++) {
		Last->next = new Vertex(Last, nullptr, TopVertices[point]);
		Last = Last->next;
		if (TopVertices[point] == TopVertices.Last()) {
			start.prev = Last;
		}
	}

	Last = &start;

	for (int point = 0; point < TopVertices.Num() - 1; point++) {
		FVector dirNext = (Last->next->point - Last->point).GetSafeNormal();
		FVector dirPrev = (Last->prev->point - Last->point).GetSafeNormal();
		float angle = FMath::Acos(FVector::DotProduct(dirNext, dirPrev));
		if (FMath::RadiansToDegrees(angle) > 180) {
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Drawing Line : %s"), *Last->prev->point.ToCompactString());
			if (Last->point.Z > Last->next->point.Z && Last->point != start.point) {
				DrawDebugLine(GetWorld(), Last->point * 10, Last->prev->point * 10, FColor::Black, true, -1, 3);
				DrawDebugLine(GetWorld(), Last->point * 10, Last->next->next->point *10, FColor::Black, true, -1, 3);
				DrawDebugLine(GetWorld(), Last->next->point *10, Last->next->next->point *10, FColor::Black, true,  -1, 3);
				Last->next = Last->next->next;
			}
			DrawDebugLine(GetWorld(), Last->point *10, Last->prev->point *10, FColor::Black, true,  -1, 3);
			DrawDebugLine(GetWorld(), Last->point *10, Last->next->point *10, FColor::Black, true, -1, 3);
			DrawDebugLine(GetWorld(), Last->next->point *10, Last->prev->point *10, FColor::Black, true, -1, 3);
		}
		Last->prev->next = Last->next;
		Last = Last->prev;
	}







}

FVector UMyMeshGenerator::ClosestPoint(FVector point, TArray<FVector> Points)
{
	FVector ClosestP;
	for (int p = 0; p < Points.Num(); p++) {
		FVector distance = point - Points[p];
		if (ClosestP.Length() > distance.Length() && Points[p] != point) {
			ClosestP = distance;
		}
	}
	return ClosestP;
}
