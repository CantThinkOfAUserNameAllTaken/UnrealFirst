

#pragma once


#include "CoreMinimal.h"
#include "ActorList.h"
#include "D:\Fork-GitStuff\FirstUnrealproject\Source\FirstUnrealproject\PlayerBullet.h"
#include "Components/ActorComponent.h"
#include "BaseAIShooting.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UBaseAIShooting : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAIShooting();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerBullet> BP_Bullet;

	UFUNCTION()
	void ShootBullet();

	UFUNCTION()
	void SpawnBullet();

	UFUNCTION()
	bool TargetFound();

	UFUNCTION()
	bool CooldownOver();

	UPROPERTY(EditAnywhere)
	float shootingInterval;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	UActorList* DA_Target;

private:

	float _timeLastShot = -888;
};
