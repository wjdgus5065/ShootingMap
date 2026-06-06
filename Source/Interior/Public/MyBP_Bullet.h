#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBP_Bullet.generated.h"

class USphereComponent;

UCLASS()
class INTERIOR_API AMyBP_Bullet : public AActor
{
	GENERATED_BODY()

public:
	AMyBP_Bullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 충돌
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComp;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 1000.f;

	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.f;

	// 이동 방향
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Direction = FVector(0.f, 0.f, 1.f);

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};