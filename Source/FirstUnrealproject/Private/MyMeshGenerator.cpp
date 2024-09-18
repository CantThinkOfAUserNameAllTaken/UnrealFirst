// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMeshGenerator.h"

void UMyMeshGenerator::CreateMesh()
{

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
	TArray<Vertex*> ConvexPoints;
	for (int point = 1; point < TopVertices.Num(); point++) {
		Last->next = new Vertex(Last, nullptr, TopVertices[point]);
		if (Last->prev != nullptr && Last->next != nullptr) {

			FVector DirPrevious = (Last->prev->point - Last->point).GetSafeNormal();
			FVector DirNext = (Last->next->point - Last->point).GetSafeNormal();
			float RadAngle = FMath::Acos(FVector::DotProduct(DirNext, DirPrevious));
			Last->angle = FMath::RadiansToDegrees(RadAngle);
			FVector crossProduct = FVector::CrossProduct(DirPrevious, DirNext);
			if (FVector::DotProduct(crossProduct, FVector::ForwardVector) < 0) {
				Last->angle = 360 - Last->angle;
			}
			if (Last->angle > 180) {

				ReflexPoints.Add(Last);
			}
			else {
				ConvexPoints.Add(Last);
			}
		}
		Last = Last->next;
		if (TopVertices[point] == TopVertices.Last()) {
			start.prev = Last;
			FVector DirPrevious = (start.prev->point - start.point).GetSafeNormal();
			FVector DirNext = (start.next->point - start.point).GetSafeNormal();
			float RadAngle = FMath::Acos(FVector::DotProduct(DirNext, DirPrevious));
			start.angle = FMath::RadiansToDegrees(RadAngle);
			FVector crossProduct = FVector::CrossProduct(DirPrevious, DirNext);
			if (FVector::DotProduct(crossProduct, FVector::ForwardVector) < 0) {
				start.angle = 360 - start.angle;
			}
			if (start.angle > 180) {
				ReflexPoints.Add(&start);
			}
			else {
				ConvexPoints.Add(Last);
			}
		}
	}

	Last = &start;

	while (!ConvexPoints.IsEmpty()) {


		if (Last->angle > 180  || AreReflexPointsInTriangle(Last))  {
			Last = Last->prev;
			UE_LOG(LogTemp, Warning, TEXT("pass 1 : %s"), *Last->point.ToCompactString());
			continue;
		}
		else {

			DrawDebugLine(GetWorld(), Last->point * 100, Last->next->point * 100, FColor::Black, true, 100, 3, 3);
			DrawDebugLine(GetWorld(), Last->point * 100, Last->prev->point * 100, FColor::Black, true, 100, 3, 3);
			DrawDebugLine(GetWorld(), Last->next->point * 100, Last->prev->point * 100, FColor::Black, true, 100, 3, 3);
			ConvexPoints.Remove(Last);
		}
		Last->prev->next = Last->next;
		Last = Last->prev;
		Last->next->prev = Last;
		for (int point = 0; point < ReflexPoints.Num(); point++) {
			if (ReflexPoints[point] == nullptr) {
				continue;
			}
			FVector DirPrevious = (ReflexPoints[point]->prev->point - ReflexPoints[point]->point).GetSafeNormal();
			FVector DirNext = (ReflexPoints[point]->next->point - ReflexPoints[point]->point).GetSafeNormal();
			float RadAngle = FMath::Acos(FVector::DotProduct(DirNext, DirPrevious));
			ReflexPoints[point]->angle = FMath::RadiansToDegrees(RadAngle);
			FVector crossProduct = FVector::CrossProduct(DirPrevious, DirNext);
			if (FVector::DotProduct(crossProduct, FVector::ForwardVector) < 0) {
				ReflexPoints[point]->angle = 360 - ReflexPoints[point]->angle;
			}
			if (ReflexPoints[point]->angle < 180) {
				ConvexPoints.Add(ReflexPoints[point]);
				ReflexPoints[point] = nullptr;
			}
		}
	}







}

bool UMyMeshGenerator::AreReflexPointsInTriangle(Vertex* Last)
{
	for (int point = 0; point < ReflexPoints.Num(); point++) {
		if (ReflexPoints[point] != nullptr && DoesPointContainTriangle(Last, ReflexPoints[point]->point)) {
			return true;
		}
	}
	return false;
}

bool UMyMeshGenerator::DoesPointContainTriangle(UMyMeshGenerator::Vertex* Last, FVector point)
{
	FVector DirPrevious = (Last->prev->point - Last->point).GetSafeNormal();
	FVector DirNext = (Last->point - Last->next->point).GetSafeNormal();
	FVector DirToPreviousFromNext = (Last->next->point - Last->prev->point).GetSafeNormal();
	FVector PointFromPrev = (point - Last->prev->point).GetSafeNormal();
	FVector PointFromNext = (point - Last->next->point).GetSafeNormal();
	FVector PointFromOrigin = (point - Last->point).GetSafeNormal();
	float deternamentPrev = FMath::Sign(CalculateWedge(DirPrevious, PointFromOrigin));
	float deternamentNext = FMath::Sign(CalculateWedge(DirNext, PointFromNext));
	UE_LOG(LogTemp, Warning, TEXT("pass 1 : %0.5f 2 : %0.5f"), deternamentPrev, deternamentNext);
	return (deternamentPrev == deternamentNext && deternamentPrev == FMath::Sign(CalculateWedge(DirToPreviousFromNext, PointFromPrev)));
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

float UMyMeshGenerator::CalculateWedge(FVector Dir1, FVector point)
{
	return (Dir1.Y * point.Z) - (Dir1.Z * point.Y);
}
