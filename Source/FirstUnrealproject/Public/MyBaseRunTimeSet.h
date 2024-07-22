// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */


template <typename T>
class FIRSTUNREALPROJECT_API MyBaseRunTimeSet {


public:
	void RegisterObject(T* Object);

	UFUNCTION()
	void DeregisterObject(T* Object);

	UFUNCTION()
	TArray<T*>& GetRegisteredObjects();

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<T*> RegisteredObjects;
	UPROPERTY(VisibleAnywhere)
	bool firstTime = false;


};


template<typename T>
inline void MyBaseRunTimeSet<T>::RegisterObject(T* Object)
{
	//if (firstTime == false) {
	//	RegisteredObjects = TArray<T*>();
	//	firstTime = true;
	//}
	RegisteredObjects.Add(Object);
}

template<typename T>
inline void MyBaseRunTimeSet<T>::DeregisterObject(T* Object)
{
	//if (firstTime == false) {
	//	RegisteredObjects = TArray<T*>();
	//	firstTime = true;
	//	return;
	//}
	RegisteredObjects.Remove(Object);
}

template<typename T>
inline TArray<T*>& MyBaseRunTimeSet<T>::GetRegisteredObjects()
{
	return RegisteredObjects;
}
